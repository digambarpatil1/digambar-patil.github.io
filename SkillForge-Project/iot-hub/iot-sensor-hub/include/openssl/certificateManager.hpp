#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <sys/stat.h>   // mkdir, stat
#include <sys/types.h>  // sometimes needed for mkdir
#include <arpa/inet.h>  // inet_pton, AF_INET, AF_INET6

#include "nlohmann/json.hpp"
#include "openssl/certificateUtils.hpp"
#include "openssl/tlsContext.hpp"
#include <openssl/x509v3.h>

using json = nlohmann::json;

/**
 * @brief CertificateManager handles generation, loading, and management
 *        of TLS certificates for MQTT broker or clients.
 *
 * This class:
 *   - Generates CA, server, and client certificates if missing
 *   - Creates a TLS context for secure MQTT connections
 *   - Supports SAN (Subject Alternative Names) for certificates
 */
class CertificateManager {
public:
    /**
     * @brief Construct a new CertificateManager
     * @param config JSON configuration containing certificates info
     */
    CertificateManager(const json& config) : config_(config) {}

    /**
     * @brief Generate CA, server, and client certificates
     * @return true if all certificates were successfully generated or already exist
     * @return false on failure
     */
    bool generateCertificates() {
        std::string dir = "/etc/mosquitto/certs/";
        if (!generateCertsFolder(dir)) return false;

        if (!config_.contains("certificates")) return false;

        // Find CA configuration
        json caConfig;
        bool hasCA = false;
        for (const auto& cert : config_["certificates"]) {
            if (cert["type"] == "CA") { 
                caConfig = cert; 
                hasCA = true; 
                break; 
            }
        }
        if (!hasCA) { 
            std::cerr << "No CA certificate defined\n"; 
            return false; 
        }

        // Generate CA certificate if missing
        if (!std::ifstream(caConfig["keyFile"]) || !std::ifstream(caConfig["certFile"])) {
            if (!certUtils::generateCertAndKey(
                    caConfig["keyFile"], 
                    caConfig["certFile"],
                    caConfig["commonName"],
                    caConfig.value("bits", 4096),
                    caConfig.value("days", 3650))) 
            {
                std::cerr << "Failed to generate CA\n";
                return false;
            }
        }

        // Load CA certificate and key
        std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> caKey(
            certUtils::loadPrivateKey(caConfig["keyFile"]), EVP_PKEY_free);
        std::unique_ptr<X509, decltype(&X509_free)> caCert(
            certUtils::loadCert(caConfig["certFile"]), X509_free);
        if (!caKey || !caCert) { 
            std::cerr << "Failed to load CA key/cert\n"; 
            return false; 
        }

        // Generate server/client certificates signed by CA
        for (const auto& cert : config_["certificates"]) {
            if (cert["type"] == "CA") continue;
            if (!std::ifstream(cert["keyFile"]) || !std::ifstream(cert["certFile"])) {
                if (!generateSignedCert(cert, caCert.get(), caKey.get())) return false;
            }
        }

        return true;
    }

    /**
     * @brief Create TLS context for MQTT client or server
     * @return TlsContext object configured with CA, cert, and key files
     */
    TlsContext createTlsContext() {
        std::string caFile, certFile, keyFile;

        for (const auto& cert : config_["certificates"]) {
            if (cert["type"] == "CA") caFile = cert["certFile"];
            else if (cert["type"] == "Client") { 
                certFile = cert["certFile"]; 
                keyFile = cert["keyFile"]; 
            }
        }

        if (caFile.empty() || certFile.empty() || keyFile.empty())
            throw std::runtime_error("TLS configuration missing");

        return TlsContext(caFile, certFile, keyFile);
    }

private:
    json config_;

    /**
     * @brief Create certificates folder if it does not exist
     * @param dir Directory path
     * @return true on success
     * @return false on failure
     */
    bool generateCertsFolder(const std::string& dir) {
        struct stat st;
        if (stat(dir.c_str(), &st) != 0) {
            if (mkdir(dir.c_str(), 0755) != 0) { 
                perror("mkdir failed"); 
                return false; 
            }
            std::cout << "Folder created: " << dir << std::endl;
        }
        return true;
    }

    /**
     * @brief Generate a certificate signed by the CA
     * @param cert JSON config for the certificate
     * @param caCert Pointer to CA X509 certificate
     * @param caKey Pointer to CA private key
     * @return true on success
     * @return false on failure
     */
    bool generateSignedCert(const json& cert, X509* caCert, EVP_PKEY* caKey) {
        // Generate key for certificate
        EVP_PKEY* pkey = certUtils::generateKey(cert.value("bits", 4096));
        if (!pkey) return false;
    
        // Create X509 certificate
        X509* x509 = X509_new();
        if (!x509) { 
            EVP_PKEY_free(pkey); 
            return false; 
        }
    
        // Set serial number and validity period
        ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
        X509_gmtime_adj(X509_get_notBefore(x509), 0);
        X509_gmtime_adj(X509_get_notAfter(x509), 60L*60L*24L*cert.value("days", 3650));
        X509_set_pubkey(x509, pkey);
    
        // Set subject name
        X509_NAME* name = X509_get_subject_name(x509);
        X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC,
                                   reinterpret_cast<const unsigned char*>("US"), -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC,
                                   reinterpret_cast<const unsigned char*>("MyOrg"), -1, -1, 0);
        std::string commonName = cert.value("commonName", "example.com");
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                                   reinterpret_cast<const unsigned char*>(commonName.c_str()), -1, -1, 0);
    
        // Set issuer name from CA
        X509_set_issuer_name(x509, X509_get_subject_name(caCert));
    
        // Add SANs if present
        if (cert.contains("san")) {
            std::string sanString;
            for (const auto& s : cert["san"]) {
                if (!sanString.empty()) sanString += ",";
                sanString += s.get<std::string>(); // e.g., "DNS:example.com" or "IP:127.0.0.1"
            }
        
            X509V3_CTX ctx;
            X509V3_set_ctx(&ctx, caCert, x509, nullptr, nullptr, 0);
            X509_EXTENSION* ext = X509V3_EXT_conf_nid(nullptr, &ctx, NID_subject_alt_name, sanString.c_str());
            if (ext) {
                X509_add_ext(x509, ext, -1);
                X509_EXTENSION_free(ext);
            } else {
                std::cerr << "Failed to create SAN extension\n";
            }
        }
    
        // Sign certificate with CA
        if (!X509_sign(x509, caKey, EVP_sha256())) {
            std::cerr << "Failed to sign " << commonName << "\n";
            EVP_PKEY_free(pkey); 
            X509_free(x509); 
            return false;
        }
    
        // Save private key and certificate to files
        if (!certUtils::savePrivateKey(pkey, cert["keyFile"]) ||
            !certUtils::saveCert(x509, cert["certFile"])) 
        {
            EVP_PKEY_free(pkey); 
            X509_free(x509); 
            return false;
        }
    
        EVP_PKEY_free(pkey);
        X509_free(x509);
        return true;
    }
};

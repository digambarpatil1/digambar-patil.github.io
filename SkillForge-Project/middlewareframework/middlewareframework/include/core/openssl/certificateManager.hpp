#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <sys/stat.h>   // mkdir, stat
#include <sys/types.h>  // sometimes needed for mkdir
#include <arpa/inet.h>  // inet_pton, AF_INET, AF_INET6

#include "nlohmann/json.hpp"
#include "certificateUtils.hpp"
#include "tlsContext.hpp"
#include <openssl/x509v3.h>

using json = nlohmann::json;

/**
 * @class CertificateManager
 * @brief Handles generation, loading, and management of TLS certificates.
 *
 * Responsible for creating CA, server, and client certificates, ensuring
 * directories exist, signing certificates, and creating a TLS context
 * for secure MQTT communication.
 */
class CertificateManager {
public:
    /**
     * @brief Construct a new CertificateManager
     * @param config JSON configuration containing certificates info
     */
    CertificateManager(const json& config) : config_(config) {}

    /**
     * @brief Generate CA, server, and client certificates if missing.
     *
     * Checks if certificates exist. If missing, generates them using
     * the configuration in JSON. Also ensures the certificates folder exists.
     *
     * @return true If all certificates are present or successfully generated.
     * @return false On any failure.
     */
    bool generateCertificates();

    /**
     * @brief Create TLS context for MQTT client or server.
     *
     * Uses the certificates from the configuration (CA, client cert, client key)
     * to build a TLS context suitable for MQTT secure connections.
     *
     * @return TlsContext Configured TLS context.
     * @throws std::runtime_error if configuration is missing required files.
     */
    TlsContext createTlsContext();

private:
    json config_; ///< JSON configuration storing certificate paths and parameters.

    /**
     * @brief Ensure the certificates folder exists, create if missing.
     * 
     * @param dir Directory path
     * @return true on success
     * @return false on failure
     */
    bool generateCertsFolder(const std::string& dir);

    /**
     * @brief Generate a certificate signed by the CA.
     *
     * @param cert JSON object containing certificate info (paths, CN, SAN, validity, bits)
     * @param caCert Pointer to CA certificate
     * @param caKey Pointer to CA private key
     * @return true On success
     * @return false On failure
     */
    bool generateSignedCert(const json& cert, X509* caCert, EVP_PKEY* caKey);
};

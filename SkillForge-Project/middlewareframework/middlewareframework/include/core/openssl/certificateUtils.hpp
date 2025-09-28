#ifndef CERTIFICATE_UTILS_H
#define CERTIFICATE_UTILS_H

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <iostream>
#include <string>

namespace certUtils {

/**
 * @brief Generate an RSA key pair wrapped in an EVP_PKEY object.
 *
 * Uses OpenSSL EVP APIs to generate an RSA private key with the specified key size.
 *
 * @param bits Key size in bits (default: 4096).
 * @return EVP_PKEY* Pointer to the generated key (caller must free with EVP_PKEY_free()).
 */
inline EVP_PKEY* generateKey(int bits = 4096) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        std::cerr << "Failed to create EVP_PKEY_CTX\n";
        return nullptr;
    }

    EVP_PKEY* pkey = nullptr;

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        std::cerr << "EVP_PKEY_keygen_init failed\n";
        EVP_PKEY_CTX_free(ctx);
        return nullptr;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        std::cerr << "EVP_PKEY_CTX_set_rsa_keygen_bits failed\n";
        EVP_PKEY_CTX_free(ctx);
        return nullptr;
    }

    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        std::cerr << "EVP_PKEY_keygen failed\n";
        EVP_PKEY_CTX_free(ctx);
        return nullptr;
    }

    EVP_PKEY_CTX_free(ctx);
    return pkey;  // Caller owns pkey, must EVP_PKEY_free()
}

/**
 * @brief Save a private key to a PEM file.
 *
 * Writes the given EVP_PKEY private key into a PEM-formatted file.
 *
 * @param pkey Pointer to the EVP_PKEY containing the private key.
 * @param filename Path to the file where the key should be written.
 * @return true on success, false on failure.
 */
inline bool savePrivateKey(EVP_PKEY* pkey, const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        std::cerr << "Cannot open file for writing: " << filename << "\n";
        return false;
    }
    bool ok = PEM_write_PrivateKey(fp, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(fp);
    return ok;
}

/**
 * @brief Load a private key from a PEM file.
 *
 * Reads an EVP_PKEY private key from a PEM file.
 *
 * @param filename Path to the PEM private key file.
 * @return EVP_PKEY* Pointer to the loaded private key (caller must free with EVP_PKEY_free()).
 */
inline EVP_PKEY* loadPrivateKey(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        std::cerr << "Cannot open private key file: " << filename << "\n";
        return nullptr;
    }
    EVP_PKEY* pkey = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return pkey;
}

/**
 * @brief Create a self-signed X.509 certificate.
 *
 * Generates a self-signed certificate using the provided private key and common name.
 *
 * @param pkey Private key used to sign the certificate.
 * @param commonName Common Name (CN) for the certificate subject.
 * @param days Validity period in days (default: 3650).
 * @return X509* Pointer to the generated certificate (caller must free with X509_free()).
 */
inline X509* createSelfSignedCert(EVP_PKEY* pkey, const std::string& commonName, int days = 3650) {
    X509* x509 = X509_new();
    if (!x509) return nullptr;

    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 60L * 60L * 24L * days);

    X509_set_pubkey(x509, pkey);

    // Subject/Issuer (for CA or self-signed certs)
    X509_NAME* name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC,
                               reinterpret_cast<const unsigned char*>("US"), -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC,
                               reinterpret_cast<const unsigned char*>("MyOrg"), -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                               reinterpret_cast<const unsigned char*>(commonName.c_str()), -1, -1, 0);

    // Self-signed (issuer = subject)
    X509_set_issuer_name(x509, name);

    if (!X509_sign(x509, pkey, EVP_sha256())) {
        std::cerr << "Failed to sign certificate\n";
        X509_free(x509);
        return nullptr;
    }

    return x509;
}

/**
 * @brief Save an X.509 certificate to a PEM file.
 *
 * Writes the given certificate in PEM format to disk.
 *
 * @param cert Pointer to the X509 certificate.
 * @param filename Path to the file where the certificate should be saved.
 * @return true on success, false on failure.
 */
inline bool saveCert(X509* cert, const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        std::cerr << "Cannot open file for writing: " << filename << "\n";
        return false;
    }
    bool ok = PEM_write_X509(fp, cert);
    fclose(fp);
    return ok;
}

/**
 * @brief Load an X.509 certificate from a PEM file.
 *
 * Reads an X509 certificate from a PEM file.
 *
 * @param filename Path to the PEM certificate file.
 * @return X509* Pointer to the loaded certificate (caller must free with X509_free()).
 */
inline X509* loadCert(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        std::cerr << "Cannot open certificate file: " << filename << "\n";
        return nullptr;
    }
    X509* cert = PEM_read_X509(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return cert;
}

/**
 * @brief Generate a private key and a self-signed certificate, and save them to files.
 *
 * This convenience function performs the following steps:
 *   - Generate an RSA private key
 *   - Save the key to a PEM file
 *   - Create a self-signed certificate with the key
 *   - Save the certificate to a PEM file
 *
 * @param keyFile Path to save the generated private key.
 * @param certFile Path to save the generated certificate.
 * @param commonName Common Name (CN) for the certificate subject.
 * @param bits Key size in bits (default: 4096).
 * @param days Validity period in days (default: 3650).
 * @return true on success, false on failure.
 */
inline bool generateCertAndKey(const std::string& keyFile,
                               const std::string& certFile,
                               const std::string& commonName,
                               int bits = 4096,
                               int days = 3650) 
{
    bool success = false;
    EVP_PKEY* pkey = nullptr;
    X509* cert = nullptr;

    do {
        pkey = generateKey(bits);
        if (!pkey) break;

        if (!savePrivateKey(pkey, keyFile)) break;

        cert = createSelfSignedCert(pkey, commonName, days);
        if (!cert) break;

        if (!saveCert(cert, certFile)) break;

        success = true;
    } while (false);

    if (cert) X509_free(cert);
    if (pkey) EVP_PKEY_free(pkey);

    return success;
}

} // namespace certUtils

#endif // CERTIFICATE_UTILS_H

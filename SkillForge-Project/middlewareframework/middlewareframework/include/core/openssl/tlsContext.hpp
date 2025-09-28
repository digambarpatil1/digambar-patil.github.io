#pragma once
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdexcept>
#include <string>

/**
 * @class TlsContext
 * @brief Wrapper around OpenSSL SSL_CTX for TLS client connections.
 *
 * Manages loading CA, client certificate, and private key,
 * and enforces TLS 1.3 for secure communication.
 */
class TlsContext {
public:
    /**
     * @brief Construct a new TlsContext
     * 
     * Loads CA, client certificate, and private key files.
     * Configures the SSL_CTX with TLS 1.3 and peer verification.
     *
     * @param ca Path to the CA certificate
     * @param cert Path to the client certificate
     * @param key Path to the client private key
     * @throws std::runtime_error if any SSL_CTX configuration fails
     */
    TlsContext(const std::string& ca,
               const std::string& cert,
               const std::string& key);

    /**
     * @brief Destroy the TlsContext
     *
     * Frees the underlying SSL_CTX.
     */
    ~TlsContext();

    /**
     * @brief Get the raw SSL_CTX pointer
     * @return SSL_CTX* Pointer to the SSL context
     */
    SSL_CTX* get() const { return ctx_; }

    /**
     * @brief Get path to CA certificate
     * @return const std::string& CA certificate file path
     */
    const std::string& getCaPath() const { return ca_; }

    /**
     * @brief Get path to client certificate
     * @return const std::string& Client certificate file path
     */
    const std::string& getCertPath() const { return cert_; }

    /**
     * @brief Get path to client private key
     * @return const std::string& Client key file path
     */
    const std::string& getKeyPath() const { return key_; }

private:
    SSL_CTX* ctx_;       ///< OpenSSL SSL_CTX pointer
    std::string ca_;     ///< Path to CA certificate
    std::string cert_;   ///< Path to client certificate
    std::string key_;    ///< Path to client private key
};

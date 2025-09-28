#pragma once
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdexcept>
#include <string>

class TlsContext {
public:
    TlsContext(const std::string& ca,
               const std::string& cert,
               const std::string& key) : ca_(ca), cert_(cert), key_(key)
    {
        ctx_ = SSL_CTX_new(TLS_client_method());
        if (!ctx_) throw std::runtime_error("Failed to create SSL_CTX");

        SSL_CTX_set_min_proto_version(ctx_, TLS1_3_VERSION);
        SSL_CTX_set_max_proto_version(ctx_, TLS1_3_VERSION);

        if (SSL_CTX_load_verify_locations(ctx_, ca.c_str(), nullptr) != 1)
            throw std::runtime_error("Failed to load CA cert");

        if (SSL_CTX_use_certificate_file(ctx_, cert.c_str(), SSL_FILETYPE_PEM) <= 0 ||
            SSL_CTX_use_PrivateKey_file(ctx_, key.c_str(), SSL_FILETYPE_PEM) <= 0)
            throw std::runtime_error("Failed to load client certificate/key");

        if (!SSL_CTX_check_private_key(ctx_))
            throw std::runtime_error("Private key does not match cert");

        SSL_CTX_set_verify(ctx_, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
    }

    ~TlsContext() { if (ctx_) SSL_CTX_free(ctx_); }

    SSL_CTX* get() const { return ctx_; }
    const std::string& getCaPath() const { return ca_; }
    const std::string& getCertPath() const { return cert_; }
    const std::string& getKeyPath() const { return key_; }

private:
    SSL_CTX* ctx_;
    std::string ca_;
    std::string cert_;
    std::string key_;
};

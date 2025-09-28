#pragma once
#include <string>
#include <openssl/ssl.h>
#include "core/messagebus.hpp"
#include "openssl/tlsContext.hpp"

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string name() const = 0;
    virtual void initialize(MessageBus& bus)=0;
    virtual void connect(TlsContext&) {};
    virtual void shutdown() {};
};
// Base class for all plugins
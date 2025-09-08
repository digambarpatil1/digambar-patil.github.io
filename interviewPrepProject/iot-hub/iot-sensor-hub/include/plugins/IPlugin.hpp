#pragma once
#include <string>
#include "core/messagebus.hpp"

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string name() const = 0;
    virtual void initialize(MessageBus& bus)=0;
    virtual void connect() {};
    virtual void shutdown() {};
};
// Base class for all plugins
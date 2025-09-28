#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "core/messagebus.hpp"

enum class PluginType {
    Unknown,
    Sensor,
    Logger,
    MQTTAdapter
};

class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual std::string name() const = 0;

    // Called once when plugin is loaded
    virtual void initialize(MessageBus& bus, nlohmann::json& config)=0;

    // Called before plugin is unloaded
    virtual void shutdown() = 0;
    virtual PluginType getType() const = 0;
};

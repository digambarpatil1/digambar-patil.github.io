#pragma once
#include "Iplugins/IPlugin.h"
#include "sensors/ISensor.hpp"
#include "core/messagebus.hpp"
#include <memory>
#include <nlohmann/json.hpp>

class ISensorPlugin : public IPlugin {
public:
    virtual ~ISensorPlugin() = default;

    // Create sensor instance from JSON config
    virtual std::unique_ptr<ISensor> createSensor(
        MessageBus& bus,
        const nlohmann::json& config) = 0;
};

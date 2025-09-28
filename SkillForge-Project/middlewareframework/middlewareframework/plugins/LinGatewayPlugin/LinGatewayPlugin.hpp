#pragma once
#include "Iplugins/ISensorPlugin.h"
#include "sensors/LinGatewaySensor.hpp"
#include "lin/LinBusSim.hpp"
#include "lin/driver/SimLinDriver.hpp"
#include "lin/LinScheduler.hpp"
#include "lin/LinNormalizer.hpp"

/**
 * @class LinGatewayPlugin
 * @brief Sensor plugin for creating LIN gateway sensors.
 *
 * Parses JSON configuration to set up polling intervals, frame schedules,
 * and signal mappings. Creates a LinGatewaySensor instance connected
 * to a simulated LIN bus.
 */
class LinGatewayPlugin : public ISensorPlugin {
public:
    /**
     * @brief Return the plugin name.
     * 
     * @return std::string Identifier for this plugin.
     */
    std::string name() const override { return "LinGatewayPlugin"; }

    /**
     * @brief Initialize the plugin.
     * 
     * Currently no-op; sensor configuration is handled in createSensor.
     *
     * @param bus Reference to the application's message bus.
     * @param config JSON configuration.
     */
    void initialize(MessageBus& bus, nlohmann::json& config) override {}

    /**
     * @brief Shutdown the plugin.
     * 
     * Currently no-op; sensor cleanup handled by unique_ptr.
     */
    void shutdown() override {}

    /**
     * @brief Get the plugin type.
     *
     * @return PluginType Enum identifying plugin type as Sensor.
     */
    PluginType getType() const override { return PluginType::Sensor; }

    /**
     * @brief Create a LinGatewaySensor instance based on JSON configuration.
     *
     * Parses configuration for polling interval, schedule, and signals,
     * then constructs a LinGatewaySensor with a simulated LIN bus and driver.
     *
     * @param bus Reference to the application's message bus.
     * @param config JSON configuration.
     * @return std::unique_ptr<ISensor> Constructed sensor instance.
     */
    std::unique_ptr<ISensor> createSensor(
        MessageBus& bus,
        const nlohmann::json& config) override;
};

/**
 * @name C-style plugin exports
 * @{
 */

/**
 * @brief Factory function to create a LinGatewayPlugin instance.
 *
 * @return IPlugin* Newly allocated LinGatewayPlugin instance.
 */
extern "C" IPlugin* create_plugin();

/**
 * @brief Destroy a LinGatewayPlugin instance.
 *
 * @param p Pointer to the plugin instance to delete.
 */
extern "C" void destroy_plugin(IPlugin* p);

/** @} */

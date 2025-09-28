#pragma once
#include "core/messagebus.hpp"
#include "Iplugins/IPlugin.h"
#include "core/Logger.hpp"

/**
 * @class LoggingPlugin
 * @brief Plugin that logs all messages from the MessageBus to a file.
 *
 * Uses the Logger singleton to record messages received via subscription.
 * Implements the IPlugin interface for dynamic plugin loading.
 */
class LoggingPlugin : public IPlugin {
private:
    Logger& logger;  ///< Reference to Logger singleton used for logging

public:
    /**
     * @brief Constructor with Logger injection.
     * 
     * @param log Reference to the Logger singleton.
     */
    explicit LoggingPlugin(Logger& log) 
        : logger(log) {}

    /**
     * @brief Return the plugin name.
     * 
     * @return std::string Identifier for this plugin.
     */
    std::string name() const override;

    /**
     * @brief Initialize the plugin.
     * 
     * Sets up the log file from config and subscribes to the MessageBus
     * to log incoming messages.
     *
     * @param bus Reference to the application's message bus.
     * @param config JSON configuration, expecting key "log_file".
     */
    void initialize(MessageBus& bus, nlohmann::json& config) override;

    /**
     * @brief Shutdown the plugin.
     * 
     * Logs plugin shutdown event. No other resources to clean up.
     */
    void shutdown() override;

    /**
     * @brief Get the plugin type.
     *
     * @return PluginType Enum identifying plugin type as Logger.
     */
    PluginType getType() const override { return PluginType::Logger; }

    /**
     * @brief Destructor.
     */
    ~LoggingPlugin();
};

/**
 * @name C-style plugin exports
 * @{
 */

/**
 * @brief Factory function to create a LoggingPlugin instance.
 *
 * @return IPlugin* Newly allocated LoggingPlugin instance.
 */
extern "C" IPlugin* create_plugin();

/**
 * @brief Destroy a LoggingPlugin instance.
 *
 * @param p Pointer to the plugin instance to delete.
 */
extern "C" void destroy_plugin(IPlugin* p);

/** @} */

#include "LoggingPlugin.hpp"
#include "core/Logger.hpp"
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <memory>

/**
 * @brief Return the plugin name.
 * 
 * @return std::string Identifier for this plugin.
 */
std::string LoggingPlugin::name() const {
    return "LoggingPlugin";
}

/**
 * @brief Initialize the LoggingPlugin.
 * 
 * Sets up the Logger singleton to write to the file specified in JSON config.
 * Subscribes to the MessageBus to log all incoming messages.
 * 
 * @param bus Reference to the application's message bus.
 * @param config JSON configuration, expecting key "log_file".
 */
void LoggingPlugin::initialize(MessageBus& bus, nlohmann::json& config) {
    // Extract log file path from config (default: "logs/app.log")
    std::string logFilePath = config.value("log_file", "logs/app.log");

    // Configure Logger singleton
    Logger::instance().setLogFile(logFilePath);
    Logger::instance().log_info("LoggingPlugin initialized. Logging to: " + logFilePath);
    Logger::instance().log_info("LoggingPlugin subscribed to MessageBus");

    // Subscribe to all messages on the bus
    bus.subscribe([](const Message& msg) {
        Logger::instance().log_info(fmt::format(
            "LoggingPlugin - Received from {}: {}", msg.sensorid, msg.payload.dump()));
    });
}

/**
 * @brief Shutdown the LoggingPlugin.
 *
 * Logs plugin shutdown event. No other resources to clean up.
 */
void LoggingPlugin::shutdown() {
    Logger::instance().log_info("LoggingPlugin shutting down");
}

/**
 * @brief Helper to access the Logger singleton.
 * 
 * @return Logger& Reference to the Logger instance.
 */
Logger& getLogger() {
    return Logger::instance();
}

/**
 * @brief Destructor.
 */
LoggingPlugin::~LoggingPlugin() = default;

/**
 * @brief Factory function for plugin system.
 * 
 * @return IPlugin* Newly created LoggingPlugin instance.
 */
extern "C" IPlugin* create_plugin() {
    return new LoggingPlugin(Logger::instance()); // inject singleton logger
}

/**
 * @brief Destroy plugin instance.
 * 
 * @param p Pointer to LoggingPlugin instance to delete.
 */
extern "C" void destroy_plugin(IPlugin* p) {
    delete p;
}

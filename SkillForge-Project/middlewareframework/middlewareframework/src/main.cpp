#include "core/messagebus.hpp"
#include "Iplugins/PluginManager.hpp"
#include "sensors/ISensor.hpp"
#include "Iplugins/ISensorPlugin.h"
#include "Iplugins/IPlugin.h"
#include "core/Logger.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <csignal>

/**
 * @brief Global flag indicating whether the application should keep running.
 * 
 * Modified by SIGINT (Ctrl+C) signal handler to gracefully stop the program.
 */
volatile std::sig_atomic_t running = true;

/**
 * @brief Entry point of the application.
 * 
 * This function:
 * - Registers a signal handler for Ctrl+C (SIGINT).
 * - Loads plugin configuration from a JSON file.
 * - Dynamically loads plugins via the PluginManager.
 * - Initializes and starts sensors, loggers, and MQTT adapters.
 * - Runs until SIGINT is received.
 * - Cleans up by stopping sensors and unloading plugins.
 * 
 * @return int Exit status (0 = success, non-zero = failure).
 */
int main() {
    // Register SIGINT (Ctrl+C) handler to stop the program
    std::signal(SIGINT, [](int) { running = false; });

    MessageBus bus;              ///< Central message bus for inter-component communication
    PluginManager manager;       ///< Plugin manager responsible for loading/unloading shared libraries

    // -------------------------------------------------------------------------
    // Load plugin configuration from JSON file
    // -------------------------------------------------------------------------
    nlohmann::json config;
    std::ifstream file("../config/plugins.json");
    if (!file.is_open()) {
        Logger::instance().log_error("Failed to open config/plugins.json");
        return 1;
    }
    file >> config;
    Logger::instance().log_info("Configuration loaded successfully");

    std::vector<std::unique_ptr<ISensor>> sensors; ///< Container for active sensor instances

    // -------------------------------------------------------------------------
    // Load and initialize plugins dynamically based on config
    // -------------------------------------------------------------------------
    for (auto& p : config["plugins"]) {
        // Skip disabled plugins
        if (p.value("_disabled", false)) continue;

        const std::string pluginName = p["type"];  ///< Plugin type identifier
        const std::string pluginPath = p["path"];  ///< Filesystem path to the plugin library

        Logger::instance().log_info("Attempting to load plugin: " + pluginName + 
                                    " from path: " + pluginPath);

        // Load shared library
        if (!manager.loadPlugin(pluginPath)) {
            Logger::instance().log_error("Failed to load plugin library: " + pluginPath);
            continue;
        }

        // Retrieve plugin instance
        IPlugin* pluginBase = manager.getPlugin(pluginName);
        if (!pluginBase) {
            Logger::instance().log_error("Failed to get plugin instance: " + pluginName);
            continue;
        }

        Logger::instance().log_info("Successfully loaded plugin: " + pluginName);

        // Handle plugin types
        switch (pluginBase->getType()) {
            case PluginType::Sensor: {
                /**
                 * @brief Handle Sensor plugin
                 * 
                 * Casts the base plugin to ISensorPlugin, creates a sensor instance,
                 * and stores it for later startup and shutdown.
                 */
                auto* sensorPlugin = dynamic_cast<ISensorPlugin*>(pluginBase);
                if (sensorPlugin) {
                    auto sensor = sensorPlugin->createSensor(bus, p["config"]);
                    sensors.push_back(std::move(sensor));
                    Logger::instance().log_info("Sensor plugin initialized: " + pluginName);
                }
                break;
            }
            case PluginType::Logger: {
                /**
                 * @brief Handle Logger plugin
                 * 
                 * Initializes a logger plugin that writes logs internally.
                 */
                pluginBase->initialize(bus, p["config"]);
                Logger::instance().log_info("Logger plugin initialized: " + pluginName);
                break;
            }
            case PluginType::MQTTAdapter: {
                /**
                 * @brief Handle MQTT Adapter plugin
                 * 
                 * Initializes an MQTT adapter for message bus communication.
                 */
                pluginBase->initialize(bus, p["config"]);
                Logger::instance().log_info("MQTT Adapter plugin initialized: " + pluginName);
                break;
            }
            default:
                Logger::instance().log_warning("Unknown plugin type encountered: " + pluginName);
                break;
        }
    }

    Logger::instance().log_info("Application started successfully");

    // -------------------------------------------------------------------------
    // Start all sensors
    // -------------------------------------------------------------------------
    for (auto& s : sensors) {
        s->start();
        Logger::instance().log_info("Sensor started");
    }

    // -------------------------------------------------------------------------
    // Main loop (runs until Ctrl+C is pressed)
    // -------------------------------------------------------------------------
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    Logger::instance().log_info("Shutdown signal received, stopping sensors...");

    // -------------------------------------------------------------------------
    // Stop all sensors gracefully
    // -------------------------------------------------------------------------
    for (auto& s : sensors) {
        s->stop();
        Logger::instance().log_info("Sensor stopped");
    }

    // -------------------------------------------------------------------------
    // Unload all plugins safely
    // -------------------------------------------------------------------------
    manager.unloadAll();
    Logger::instance().log_info("All plugins unloaded, application exiting.");

    return 0;
}

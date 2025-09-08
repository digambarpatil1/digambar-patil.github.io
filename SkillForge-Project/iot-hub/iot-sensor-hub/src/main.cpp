#include "sensors/dht22sensor.hpp"
#include "sensors/PIRsensor.hpp"
#include "core/messagebus.hpp"
#include "plugins/pluginManager.hpp"
#include "plugins/LoggingPlugin.hpp"
#include "plugins/filesinkplugin.hpp"
#include "logger/Logger.hpp"
#include "core/config.hpp"
#include "adapters/AdapterFactory.hpp"
#include "sensors/sensorfactory.hpp"
#include <iostream>
#include <csignal>
#include <memory>
#include <fmt/core.h>

volatile std::sig_atomic_t running = true;

int main() {
    std::signal(SIGINT, [](int) { 
        running = false;
    });

    auto config = Config::load("config/config.json");

    MessageBus bus;
    PluginManager pluginManager;

    if (config.contains("log_file")) {
        Logger::instance().setLogFile(config["log_file"].get<std::string>());
    }
    Logger::instance().log_info("Application started");

    
    // Register plugins
    for (const auto& pluginConf : config["plugins"]) {
        if ((pluginConf["_disabled"].get<bool>())) continue;
        auto plugin = AdapterFactory::create(pluginConf);
        pluginManager.registerPlugin(std::move(plugin), bus);
    }

    // Register adapters
    for (const auto& adapterConf : config["adapters"]) {
        if (adapterConf["_disabled"].get<bool>()) continue;
        auto adapter = AdapterFactory::create(adapterConf);
        adapter->connect();
        pluginManager.registerPlugin(std::move(adapter), bus);
    }
    // Initialize and start sensors
    std::vector<std::unique_ptr<ISensor>> sensors;
    for (const auto& sensorconf : config["sensors"]) {
        if (sensorconf["_disabled"].get<bool>()) continue;
         auto sensor = sensorfactory::create(bus, sensorconf);
         if (!sensor) {
             Logger::instance().log_error("Failed to create sensor from config");
             break;
         }
         sensors.push_back(std::move(sensor));
         sensors.back()->start();
    }
 
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    for(auto& sensor : sensors) {
        sensor->stop();
    }
    Logger::instance().log_info("Application ended");
    return 0;
}

#include "plugins/LoggingPlugin.hpp"
#include "logger/Logger.hpp"
#include <fmt/core.h>
#include <nlohmann/json.hpp>

std::string LoggingPlugin::name() const  {
    return "LoggingPlugin";
}

void LoggingPlugin::initialize(MessageBus& bus)  {
    bus.subscribe([](const Message& msg) {
        Logger::instance().log_info(fmt::format("LoggingPlugin - Received from {}: {}", msg.sensorid, msg.payload.dump()));
    });
    Logger::instance().log_info("LoggingPlugin initialized and subscribed to MessageBus");
}

void LoggingPlugin::shutdown()  {
    Logger::instance().log_info("LoggingPlugin shutting down");
}

LoggingPlugin::~LoggingPlugin() = default;
#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include "adapters/MQTTAdapter.hpp"
#include "adapters/HTTPAdapter.hpp"
#include "plugins/LoggingPlugin.hpp"
#include "plugins/filesinkplugin.hpp"
#include <string>

class AdapterFactory {
public:
    static std::unique_ptr<IPlugin> create(const nlohmann::json& config) {
        auto type = config.at("type").get<std::string>();
        if (type == "LoggingPlugin") {
            return std::make_unique<LoggingPlugin>();
        }
        else if (type == "FileSinkPlugin") {
            return std::make_unique<filesinkplugin>(config.at("path").get<std::string>());
        }
        else if (type == "MQTTAdapter") {
            return std::make_unique<MQTTAdapter>(config.at("broker").get<std::string>(),
            config.at("topic").get<std::string>());
        }else if(type == "HTTPAdapter")
        {
           return std::make_unique<HTTPAdapter>(config.at("endpoint").get<std::string>());
        }else 
        throw std::runtime_error("Unknown plugin type: " + type);
        
        return nullptr;
       
    }
};
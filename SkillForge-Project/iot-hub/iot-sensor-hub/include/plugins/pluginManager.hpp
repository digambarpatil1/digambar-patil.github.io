#pragma once
#include <string>
#include "plugins/IPlugin.hpp"
#include "core/messagebus.hpp"
#include <vector>
#include <memory>
class PluginManager {
public:
    PluginManager() = default;
    ~PluginManager()=default;
    inline void registerPlugin(std::unique_ptr<IPlugin> plugin ,MessageBus& bus){
        plugin->initialize(bus);
        plugins.push_back(std::move(plugin));
    }
   // bool loadPlugin(const std::string& path);
   // void unloadPlugins();
   // void shutdownPlugins();
private:
    std::vector<std::unique_ptr<IPlugin>> plugins;
  //  std::vector<void*> handles; // To store handles for dynamic libraries
};

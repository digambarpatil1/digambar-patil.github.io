#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Iplugins/IPlugin.h"
class PluginManager {
public:
    PluginManager() = default;
    ~PluginManager();

    // Load a plugin from shared object (.so)
    bool loadPlugin(const std::string& path);

    // Get a loaded plugin by name
    IPlugin* getPlugin(const std::string& name);

    // Unload all plugins
    void unloadAll();

private:
    struct PluginHandle {
        void* handle;             // dlopen handle
        IPlugin* instance;  // created plugin instance
    };

    std::unordered_map<std::string, PluginHandle> plugins_;
};

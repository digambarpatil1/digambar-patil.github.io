#include "Iplugins/PluginManager.hpp"
#include <dlfcn.h>
#include <iostream>

/**
 * @brief Type alias for plugin creation function pointer.
 * 
 * Each plugin shared library must expose `create_plugin()` returning IPlugin*.
 */
using CreatePluginFunc = IPlugin* (*)();

/**
 * @brief Type alias for plugin destruction function pointer.
 * 
 * Each plugin shared library may expose `destroy_plugin(IPlugin*)` to clean up.
 */
using DestroyPluginFunc = void (*)(IPlugin*);

/**
 * @brief Destroy the PluginManager object.
 * 
 * Ensures all loaded plugins are unloaded and shared libraries closed.
 */
PluginManager::~PluginManager() {
    unloadAll();
}

/**
 * @brief Load a plugin shared library from the specified path.
 * 
 * This function dynamically loads the shared library, looks up the
 * `create_plugin` symbol, creates an instance, and stores it internally.
 *
 * @param path Path to the plugin shared library (.so)
 * @return true if plugin successfully loaded, false otherwise
 */
bool PluginManager::loadPlugin(const std::string& path) {
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return false;
    }

    // Find the create_plugin function
    auto createFunc = (CreatePluginFunc)dlsym(handle, "create_plugin");
    if (!createFunc) {
        std::cerr << "Failed to find create_plugin in " << path << std::endl;
        dlclose(handle);
        return false;
    }

    // Create plugin instance
    IPlugin* plugin = createFunc();
    if (!plugin) {
        std::cerr << "Plugin creation failed for " << path << std::endl;
        dlclose(handle);
        return false;
    }

    std::cout << "Plugin " << plugin->name() << " loaded successfully from " << path << std::endl;

    // Store handle and instance in internal map
    plugins_[plugin->name()] = {handle, plugin};
    return true;
}

/**
 * @brief Retrieve a plugin instance by its name.
 *
 * @param name Name of the plugin to retrieve.
 * @return IPlugin* Pointer to plugin instance, or nullptr if not found.
 */
IPlugin* PluginManager::getPlugin(const std::string& name) {
    std::cout << "Searching for plugin: " << name << std::endl;
    auto it = plugins_.find(name);
    if (it != plugins_.end()) {
        return it->second.instance;
    }
    return nullptr;
}

/**
 * @brief Unload all plugins and close their shared libraries.
 * 
 * For each loaded plugin, this function calls its `destroy_plugin` function
 * if available, and then closes the shared library with `dlclose()`.
 * Clears the internal plugin map afterwards.
 */
void PluginManager::unloadAll() {
    for (auto& kv : plugins_) {
        auto& handle = kv.second.handle;
        auto& instance = kv.second.instance;

        // Call destroy_plugin if it exists
        auto destroyFunc = (DestroyPluginFunc)dlsym(handle, "destroy_plugin");
        if (destroyFunc && instance) {
            destroyFunc(instance);
        }

        // Close the shared library
        if (handle) {
            dlclose(handle);
        }
    }
    plugins_.clear();
}

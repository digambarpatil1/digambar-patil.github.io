#pragma once
#include "core/messagebus.hpp"
#include "core/openssl/tlsContext.hpp"
#include "Iplugins/IPlugin.h"
#include "core/Logger.hpp"
#include <memory>
#include <mqtt/async_client.h>

/**
 * @class MQTTAdapter
 * @brief MQTT Adapter plugin bridging MessageBus to MQTT broker.
 *
 * Connects to a broker using TLS, subscribes to a topic, and publishes
 * messages from the MessageBus to MQTT. Also consumes MQTT messages
 * and republishes them to the MessageBus.
 *
 * Implements the IPlugin interface for dynamic plugin loading.
 */
class MQTTAdapter : public IPlugin {
public:
    /**
     * @brief Default constructor.
     */
    MQTTAdapter() = default;

    /**
     * @brief Destructor.
     *
     * Ensures the MQTT client is safely disconnected and resources cleaned up.
     */
    ~MQTTAdapter() override;

    /**
     * @brief Return the plugin name.
     * 
     * @return std::string Identifier for this plugin.
     */
    std::string name() const override;

    /**
     * @brief Initialize the plugin with configuration and subscribe to the bus.
     *
     * Reads broker and topic from the config JSON, sets up TLS certificates,
     * connects to the MQTT broker, subscribes to the topic, and bridges
     * messages between MQTT and MessageBus.
     *
     * @param bus Reference to the application's message bus.
     * @param config JSON configuration with keys `broker`, `topic`, and TLS info.
     */
    void initialize(MessageBus& bus, nlohmann::json& config) override;

    /**
     * @brief Connect to the MQTT broker using TLS context.
     *
     * @param ctx TLS context containing CA, client certificate, and key paths.
     */
    void connect(TlsContext& ctx);

    /**
     * @brief Shutdown the MQTT client and disconnect from the broker.
     */
    void shutdown() override;

    /**
     * @brief Get the plugin type.
     *
     * @return PluginType Enum identifying plugin type as MQTTAdapter.
     */
    PluginType getType() const override { return PluginType::MQTTAdapter; }
    
private:
    std::string broker_;                        ///< MQTT broker address
    std::string topic_;                         ///< MQTT topic to publish/subscribe
    std::unique_ptr<mqtt::async_client> client_; ///< MQTT client instance
    TlsContext* context;                        ///< Pointer to TLS context
};

/**
 * @name C-style plugin exports
 * @{
 */

/**
 * @brief Factory function to create a plugin instance.
 *
 * @return IPlugin* Newly allocated MQTTAdapter instance.
 */
extern "C" IPlugin* create_plugin();

/**
 * @brief Destroy a plugin instance.
 *
 * @param p Pointer to the plugin instance to delete.
 */
extern "C" void destroy_plugin(IPlugin* p);

/** @} */

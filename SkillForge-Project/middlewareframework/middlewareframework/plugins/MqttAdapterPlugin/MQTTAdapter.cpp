#include "MQTTAdapter.hpp"
#include "core/Logger.hpp"
#include <mqtt/async_client.h>
#include "core/openssl/certificateManager.hpp"
#include <thread>

/**
 * @brief Destructor for MQTTAdapter.
 * Stops and cleans up MQTT client if connected.
 */
MQTTAdapter::~MQTTAdapter() {
    shutdown();
    Logger::instance().log_info("[MQTTAdapter] Destructor called, adapter disposed");
    // std::cout << "[MQTTAdapter] Destructor called, adapter disposed" << std::endl;
}

/**
 * @brief Return the plugin name.
 * 
 * @return std::string Plugin identifier
 */
std::string MQTTAdapter::name() const {
    return "MqttAdapterPlugin";
}

/**
 * @brief Initialize the adapter.
 * 
 * Sets up MQTT broker connection, TLS, subscribes to topics,
 * and bridges MessageBus <-> MQTT messages.
 * 
 * @param bus Reference to the application's message bus.
 * @param config JSON configuration containing `topic`, `broker`, and TLS info.
 */
void MQTTAdapter::initialize(MessageBus& bus, nlohmann::json& config) {
    if (config.contains("topic")) {
        topic_ = config["topic"];
    } else {
        Logger::instance().log_error("MQTTAdapterPlugin config missing 'topic'");
        return;
    }

    if (config.contains("broker")) {
        broker_ = config["broker"];
    }

    // Generate or load certificates
    CertificateManager certManager(config);
    if (!certManager.generateCertificates()) {
        Logger::instance().log_error("Certificate generation failed");
        return;
    }

    // Build TLS context
    TlsContext tlscontext = certManager.createTlsContext();

    // Connect to broker
    connect(tlscontext);

    // Bridge Bus -> MQTT (publish)
    bus.subscribe([this](const Message& msg) {
        if (!client_ || !client_->is_connected()) {
            Logger::instance().log_warning("[MQTTAdapter] Bus message dropped, client not connected");
            return;
        }

        std::string payload = msg.payload.dump();
        auto mqtt_msg = mqtt::make_message(msg.topic, payload);
        mqtt_msg->set_qos(1);

        try {
            client_->publish(mqtt_msg)->wait();
            Logger::instance().log_info("[MQTTAdapter] Published to MQTT topic: " + msg.topic + " -> " + payload);
        } catch (const mqtt::exception& e) {
            Logger::instance().log_error("[MQTTAdapter] MQTT publish error: " + std::string(e.what()));
        }
    });

    // Bridge MQTT -> Bus (consume in background thread)
    std::thread([this, &bus]() {
        while (client_ && client_->is_connected()) {
            auto msg = client_->consume_message();
            if (!msg) continue;

            Message busMsg;
            busMsg.topic = msg->get_topic();

            try {
                busMsg.payload = nlohmann::json::parse(msg->to_string());
            } catch (...) {
                busMsg.payload = msg->to_string();
            }

            bus.publish(busMsg);
            Logger::instance().log_info("[MQTTAdapter] Received from MQTT topic: " + busMsg.topic);
        }
    }).detach();
}

/**
 * @brief Connect to the MQTT broker with TLS.
 * 
 * @param ctx TLS context containing CA, cert, and key paths
 */
void MQTTAdapter::connect(TlsContext& ctx) {
    context = &ctx;
    client_ = std::make_unique<mqtt::async_client>(broker_, "MQTTAdapterClient");

    mqtt::ssl_options sslOpts;
    sslOpts.set_trust_store(ctx.getCaPath());
    sslOpts.set_key_store(ctx.getCertPath());
    sslOpts.set_private_key(ctx.getKeyPath());
    sslOpts.set_enable_server_cert_auth(true);
    sslOpts.set_ssl_version(MQTT_SSL_VERSION_TLS_1_2);

    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_ssl(sslOpts);

    try {
        client_->connect(connOpts)->wait();
        client_->start_consuming();
        client_->subscribe(topic_, 1)->wait();
        Logger::instance().log_info("[MQTTAdapter] Subscribed to topic: " + topic_);
    } catch (const mqtt::exception& e) {
        Logger::instance().log_error("[MQTTAdapter] MQTT connect error: " + std::string(e.what()));
    }
}

/**
 * @brief Disconnect from MQTT broker if connected.
 */
void MQTTAdapter::shutdown() {
    if (client_ && client_->is_connected()) {
        try {
            client_->disconnect()->wait();
            Logger::instance().log_info("[MQTTAdapter] Disconnected from MQTT broker");
        } catch (const mqtt::exception& e) {
            Logger::instance().log_error("[MQTTAdapter] MQTT disconnect error: " + std::string(e.what()));
        }
    }
}

/**
 * @brief Factory function for plugin system.
 * 
 * @return IPlugin* Newly created plugin instance
 */
extern "C" IPlugin* create_plugin() {
    return new MQTTAdapter();
}

/**
 * @brief Destroy plugin instance.
 * 
 * @param p Pointer to plugin instance to delete
 */
extern "C" void destroy_plugin(IPlugin* p) {
    delete p;
}

#include "adapters/MQTTAdapter.hpp"
#include <iostream>
#include "logger/Logger.hpp"
#include <mqtt/async_client.h>

MQTTAdapter::MQTTAdapter(const std::string& broker, const std::string& topic)
    : broker_(broker), topic_(topic) {
    std::cout << "[MQTTAdapter] Constructor called for broker: " << broker_ << std::endl;
}

MQTTAdapter::~MQTTAdapter() {
    disconnect();
    Logger::instance().log_info("[MQTTAdapter] Destructor called, adapter disposed");
    std::cout << "[MQTTAdapter] Destructor called, adapter disposed" << std::endl;
}

std::string MQTTAdapter::name() const {
    return "MQTTAdapter";
}

void MQTTAdapter::initialize(MessageBus& bus) {
    std::cout << "[MQTTAdapter] initialize() called" << std::endl;

    // Bridge Bus -> MQTT (publish)
    bus.subscribe([this](const Message& msg) {
        if (!client_ || !client_->is_connected()) {
            std::cout << "[MQTTAdapter] Bus message dropped, client not connected" << std::endl;
            return;
        }

        std::string payload = msg.payload.dump();
        auto mqtt_msg = mqtt::make_message(msg.topic, payload);
        mqtt_msg->set_qos(1);

        try {
            client_->publish(mqtt_msg)->wait();
            Logger::instance().log_info("[MQTTAdapter] Published to MQTT topic: " + msg.topic + " -> " + payload);
            std::cout << "[MQTTAdapter] Published to MQTT topic: " << msg.topic << "->" << payload << std::endl;
        } catch (const mqtt::exception& e) {
            Logger::instance().log_error("[MQTTAdapter] MQTT publish error: " + std::string(e.what()));
            std::cerr << "[MQTTAdapter] MQTT publish error: " << e.what() << std::endl;
        }
    });

    // Bridge MQTT -> Bus (consume in background)
    std::thread([this, &bus]() {
        std::cout << "[MQTTAdapter] MQTT -> Bus consumer thread started" << std::endl;
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
            std::cout << "[MQTTAdapter] Received from MQTT topic: " << busMsg.topic << std::endl;
        }
        std::cout << "[MQTTAdapter] MQTT -> Bus consumer thread exiting" << std::endl;
    }).detach();
}

void MQTTAdapter::connect(TlsContext& ctx) {
    context = &ctx;
    std::cout << "[MQTTAdapter] connect() called for broker: " << broker_ << std::endl;

    client_ = std::make_unique<mqtt::async_client>(broker_, "MQTTAdapterClient");

    // SSL/TLS options
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
        // 1️⃣ Connect
        std::cout << "[MQTTAdapter] Connecting to broker..." << broker_ << std::endl;
        client_->connect(connOpts)->wait();
        std::cout << "[MQTTAdapter] Connected to broker " << broker_ << std::endl;

        // 2️⃣ Start consuming
        client_->start_consuming();
        std::cout << "[MQTTAdapter] Started consuming messages" << std::endl;

        std::cout << "[DEBUG] topic_ = '" << topic_ << "'" << std::endl;

        // 3️⃣ Subscribe using topic_ safely
        std::string subTopic = topic_;  // ensure lifetime
        client_->subscribe(subTopic, 1)->wait();
        Logger::instance().log_info("[MQTTAdapter] Subscribed to topic: " + subTopic);
        std::cout << "[MQTTAdapter] Subscribed to topic: " << subTopic << std::endl;

    } catch (const mqtt::exception& e) {
        Logger::instance().log_error("[MQTTAdapter] MQTT connect error: " + std::string(e.what()));
        std::cerr << "[MQTTAdapter] MQTT connect error: " << e.what() << std::endl;
    }
}

void MQTTAdapter::disconnect() {
    if (client_ && client_->is_connected()) {
        try {
            client_->disconnect()->wait();
            Logger::instance().log_info("[MQTTAdapter] Disconnected from MQTT broker");
            std::cout << "[MQTTAdapter] Disconnected from MQTT broker" << std::endl;
        } catch (const mqtt::exception& e) {
            Logger::instance().log_error("[MQTTAdapter] MQTT disconnect error: " + std::string(e.what()));
            std::cerr << "[MQTTAdapter] MQTT disconnect error: " << e.what() << std::endl;
        }
    }
}

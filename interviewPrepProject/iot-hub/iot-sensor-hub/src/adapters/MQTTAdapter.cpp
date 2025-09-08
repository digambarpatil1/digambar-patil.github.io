#include "adapters/MQTTAdapter.hpp"
#include <iostream>
#include "logger/Logger.hpp"

MQTTAdapter::MQTTAdapter(const std::string& broker, const std::string& topic)
: broker_(broker), topic_(topic) {
    // Initialize MQTT client here if needed
}
std::string MQTTAdapter::name() const {
    return "MQTTAdapter";
}
void MQTTAdapter::initialize(MessageBus& bus) 
{
    bus.subscribe([this](const Message& msg) {
        // Here, you would implement the logic to publish the message to an MQTT broker.
        // For demonstration, we'll just print the message to the console.
        Logger::instance().log_info(
            "MQTTAdapter (fake) publishing to " + topic_ + ": " + msg.payload.dump()
        );
    });
    
}
void MQTTAdapter::connect() 
{
    Logger::instance().log_info("MQTTAdapter connecting to broker: " + broker_);

}
void MQTTAdapter::disconnect() 
{
    Logger::instance().log_info("MQTTAdapter disconnected");
}
MQTTAdapter::~MQTTAdapter() 
{
    Logger::instance().log_info("MQTTAdapter disposed");
}
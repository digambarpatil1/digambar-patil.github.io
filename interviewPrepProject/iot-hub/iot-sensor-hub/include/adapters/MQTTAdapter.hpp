#pragma once
#include  "plugins/IPlugin.hpp"
#include "core/messagebus.hpp"
#include <string>
class MQTTAdapter : public IPlugin {
public:
    MQTTAdapter(const std::string& broker, const std::string& topic);
    std::string name() const override;
    void initialize(MessageBus& bus) override;
    void connect();
    void disconnect();
    ~MQTTAdapter() override;
private:
    std::string broker_;
    std::string topic_; // MQTT topic to publish to
};
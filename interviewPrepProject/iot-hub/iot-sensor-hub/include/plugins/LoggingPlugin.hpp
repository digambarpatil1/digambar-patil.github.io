#pragma once
#include "plugins/IPlugin.hpp"
class LoggingPlugin : public IPlugin {
public:
    std::string name() const override;
    void initialize(MessageBus& bus) override;
    void shutdown() override;
    ~LoggingPlugin() override;
};

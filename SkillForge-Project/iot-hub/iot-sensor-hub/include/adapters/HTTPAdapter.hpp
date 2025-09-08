#pragma once
#include "plugins/IPlugin.hpp"
#include "core/messagebus.hpp"
#include "logger/Logger.hpp"

class HTTPAdapter : public IPlugin {
public:
    HTTPAdapter(const std::string& endpoint);
    std::string name() const override ;
    void initialize(MessageBus& bus) override;
    void connect() ;
    void disconnect() ;

private:
    std::string endpoint_;
};

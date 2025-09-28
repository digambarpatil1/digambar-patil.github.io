
#include "core/messagebus.hpp"
#include "adapters/HTTPAdapter.hpp"
#include "logger/Logger.hpp"    
#include <iostream>

HTTPAdapter::HTTPAdapter(const std::string& endpoint) : endpoint_(endpoint) {}

std::string HTTPAdapter::name() const  { return "HTTPAdapter"; }

void HTTPAdapter::initialize(MessageBus& bus) {
    bus.subscribe([this](const Message& msg) {
        Logger::instance().log_info(
            "HTTPAdapter (fake) posting to " + endpoint_ + ": " + msg.payload.dump()
        );
    });
}

void HTTPAdapter::connect(TlsContext& ctx){
    Logger::instance().log_info("HTTPAdapter ready at endpoint: " + endpoint_);
}

void HTTPAdapter::disconnect(){
    Logger::instance().log_info("HTTPAdapter stopped");
}

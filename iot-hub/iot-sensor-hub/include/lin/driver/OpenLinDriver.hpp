// OpenLinDriver.hpp
#pragma once
#include "ILinDriver.hpp"
//#include "open_lin.h"   // from Open-LIN C library
/*
class OpenLinDriver : public ILinDriver {
    lin_channel_t channel;
public:
    OpenLinDriver(lin_channel_t ch) : channel(ch) {}

    bool requestFrame(uint8_t frameId, std::vector<uint8_t>& data) override {
        uint8_t buf[8]; // typical LIN frame
        int len = lin_request_frame(channel, frameId, buf, sizeof(buf));
        if (len <= 0) return false;
        data.assign(buf, buf + len);
        return true;
    }

    bool sendFrame(uint8_t frameId, const std::vector<uint8_t>& data) override {
        return lin_send_frame(channel, frameId, data.data(), data.size()) == 0;
    }
};*/

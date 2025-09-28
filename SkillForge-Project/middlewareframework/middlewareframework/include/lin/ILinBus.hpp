#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "LinScheduler.hpp"

class ILinBus {
public:
    virtual ~ILinBus() = default;

    // Initialize the bus (baudrate, channel, etc.)
    virtual bool init(const std::string& channel, int baudrate) = 0;

    // Send raw frame
    virtual bool sendFrame(uint8_t id, const std::vector<uint8_t>& data) = 0;

    // Receive frame (blocking or non-blocking)
    virtual bool receiveFrame(uint8_t& id, std::vector<uint8_t>& data) = 0;

    // Scheduler integration: poll frames based on schedule
    virtual void poll(const LinScheduler& scheduler) = 0;
};

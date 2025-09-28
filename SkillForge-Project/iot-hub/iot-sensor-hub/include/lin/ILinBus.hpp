#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <cstdint>
//ILinBus: high-level LIN abstraction (polling, sending, receiving).
class ILinBus {
public:
    virtual ~ILinBus() = default;
    // e.g. channel="/dev/ttyS0" or "ttyAMA0" or "sim"
    virtual bool init(const std::string& channel, int baud) = 0;

    // Master: send header (request) for a given frame id
    virtual bool request(uint8_t frame_id) = 0;

    // Blocking receive with timeout; fills id and data (0..8 bytes)
    virtual bool receive(uint8_t& frame_id,
                         std::vector<uint8_t>& data,
                         std::chrono::milliseconds timeout) = 0;

    virtual void shutdown() = 0;
};

// ILinDriver.hpp
#pragma once
#include <vector>
#include <cstdint>
//ILinDriver: low-level driver abstraction (UART / OpenLIN / simulator).
class ILinDriver {
public:
    virtual ~ILinDriver() = default;

    // Send a frame request (header + checksum) and receive data bytes
    virtual bool requestFrame(uint8_t frameId, std::vector<uint8_t>& data) = 0;

    // Optionally send a frame (if acting as slave/master publishing data)
    virtual bool sendFrame(uint8_t frameId, const std::vector<uint8_t>& data) = 0;
};

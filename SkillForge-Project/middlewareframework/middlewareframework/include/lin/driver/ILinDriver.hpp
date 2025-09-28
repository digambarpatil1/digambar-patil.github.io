#pragma once
#include <vector>
#include <cstdint>
#include <string>
//ILinDriver: low-level driver abstraction (UART / OpenLIN / simulator).
class ILinDriver {
public:
    virtual ~ILinDriver() = default;

    virtual bool open(const std::string& channel, int baudrate) = 0;
    virtual void close() = 0;

    virtual bool send(const std::vector<uint8_t>& frame) = 0;
    virtual bool receive(std::vector<uint8_t>& frame) = 0;
};
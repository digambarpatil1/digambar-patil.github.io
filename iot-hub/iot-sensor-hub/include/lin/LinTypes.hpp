#pragma once
#include <array>
#include <chrono>
#include <cstdint>

struct LinFrame {
    uint8_t id{};
    std::array<uint8_t, 8> data{};
    uint8_t dlc{8};
    std::chrono::system_clock::time_point ts;
};

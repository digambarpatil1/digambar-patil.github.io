// lin/LinNormalizer.hpp
#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "lin/LinTypes.hpp"

struct SignalSpec {
    uint8_t frame_id;        // LIN frame id
    std::string name;        // e.g. "climate.temp_c"
    size_t byte;             // start byte (0..7) – byte-aligned for v1
    size_t len;              // 1 or 2 (support u8, bool, u16, i16)
    std::string type;        // "u8","bool","u16","i16"
    double scale{1.0};       // y = raw * scale + offset
    double offset{0.0};
    std::string endian{"little"};
};

class LinNormalizer {
public:
    explicit LinNormalizer(std::vector<SignalSpec> specs) : specs_(std::move(specs)) {}

    // Returns flat JSON: {"signals": { name: value, ... }, "frame_id":X, "ts":...}
    nlohmann::json normalize(const LinFrame& f) const;

private:
    std::vector<SignalSpec> specs_;
};

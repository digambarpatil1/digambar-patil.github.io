// SimLinDriver.hpp
#pragma once
#include "ILinDriver.hpp"
#include <random>

class SimLinDriver : public ILinDriver {
public:
    bool requestFrame(uint8_t frameId, std::vector<uint8_t>& data) override {
        // Example: simulate 2-byte frames
        data.clear();
        if (frameId == 16) { // climate
            data.push_back(20 + rand() % 10); // temp 20–29
            data.push_back(40 + rand() % 30); // humidity 40–70
        } else if (frameId == 17) { // seat
            data.push_back(rand() % 2); // on/off
        }
        return true;
    }

    bool sendFrame(uint8_t, const std::vector<uint8_t>&) override {
        return true; // Not needed for now
    }
};

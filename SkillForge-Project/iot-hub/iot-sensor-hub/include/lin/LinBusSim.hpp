#pragma once
#include "lin/ILinBus.hpp"
#include <optional>
#include <random>

class LinBusSim : public ILinBus {
public:
    bool init(const std::string& channel, int baud); 
    bool request(uint8_t frame_id);
    bool receive(uint8_t& frame_id, std::vector<uint8_t>& data,
                 std::chrono::milliseconds timeout);

    void shutdown();

private:
    std::optional<uint8_t> last_id_;
    uint8_t buffer_[8]{};
    bool ready_{false};
    std::mt19937 rng_;
    std::uniform_int_distribution<int> dist_{0,255};
};

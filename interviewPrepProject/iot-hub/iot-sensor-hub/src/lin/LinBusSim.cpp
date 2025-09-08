#pragma once
#include "lin/ILinBus.hpp"
#include  "lin/LinBusSim.hpp"
#include <optional>
#include <random>

 
bool LinBusSim::init(const std::string &channel, int baud)
{
    (void)channel;
    (void)baud;
    rng_.seed(12345);
    return true;
}

bool LinBusSim::request(uint8_t frame_id)
{
    last_id_ = frame_id;
    // synthesize 8 bytes
    for (auto &b : buffer_)
        b = static_cast<uint8_t>(dist_(rng_) & 0xFF);
    // make some stable-ish patterns by frame_id
    buffer_[0] = static_cast<uint8_t>(20 + (frame_id % 10)); // temp-ish
    buffer_[1] = static_cast<uint8_t>(50 + (frame_id % 40)); // humidity-ish
    ready_ = true;
    return true;
}

bool LinBusSim::receive(uint8_t &frame_id, std::vector<uint8_t> &data,
                        std::chrono::milliseconds timeout)
{
    (void)timeout; // immediate in sim
    if (!ready_ || !last_id_.has_value())
        return false;
    frame_id = *last_id_;
    data.assign(buffer_, buffer_ + 8);
    ready_ = false;
    return true;
}

void LinBusSim::shutdown()
{
    ready_ = false;
    last_id_.reset();
}

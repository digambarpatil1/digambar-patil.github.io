#pragma once
#include "ILinBus.hpp"
#include "lin/driver/ILinDriver.hpp"
#include "LinScheduler.hpp"
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

class LinBusSim : public ILinBus {
public:
    LinBusSim();
    ~LinBusSim() override;

    bool init(const std::string& channel, int baudrate) override;
    bool sendFrame(uint8_t id, const std::vector<uint8_t>& data) override;
    bool receiveFrame(uint8_t& id, std::vector<uint8_t>& data) override;
    void poll(const LinScheduler& scheduler) override;

private:
    std::unique_ptr<ILinDriver> driver_;
    std::thread pollThread_;
    std::atomic<bool> running_;
    LinScheduler scheduler_;
    void pollLoop();
};

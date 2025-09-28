#pragma once
#include "ISensor.hpp"
#include "core/messagebus.hpp"
#include "lin/ILinBus.hpp"
#include "lin/LinScheduler.hpp"
#include "lin/LinNormalizer.hpp"
#include "lin/driver/ILinDriver.hpp"
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

class LinGatewaySensor : public ISensor {
public:
    LinGatewaySensor(MessageBus& bus,
                     std::unique_ptr<ILinBus> linBus,
                     std::unique_ptr<ILinDriver> driver,
                     LinScheduler scheduler,
                     LinNormalizer normalizer,
                     int poll_interval_ms);

    ~LinGatewaySensor() override;

    void start() override;
    void stop() override;

private:
    void pollingLoop();

    MessageBus& bus_;
    std::unique_ptr<ILinBus> linBus_;
    std::unique_ptr<ILinDriver> driver_;
    LinScheduler scheduler_;
    LinNormalizer normalizer_;
    int poll_interval_ms_;

    std::thread pollThread_;
    std::atomic<bool> running_;
};

#pragma once
#include "lin/ILinBus.hpp"
#include "lin/LinSchedule.hpp"
#include "lin/LinNormalizer.hpp"
#include "lin/LinTypes.hpp"
#include "core/messagebus.hpp"
#include "logger/Logger.hpp"
#include "sensors/ISensor.hpp" // your ISensor with start/stop
#include "lin/driver/ILinDriver.hpp"
#include <thread>
#include <atomic>
#include <memory>

class LinGatewaySensor : public ISensor {
public:
    LinGatewaySensor(MessageBus& bus,
                     std::unique_ptr<ILinBus> hal,
                     std::string channel,
                     int baudrate,
                     LinScheduler scheduler,
                     LinNormalizer normalizer,
                     int rx_timeout_ms = 20, std::unique_ptr<ILinDriver> driver=nullptr);

    ~LinGatewaySensor() override;

    std::string getSensorName() const ;

    // No direct readData() in the threaded model; we publish from the loop.
    //SensorData readData() override { return {"LIN", 0.0, std::chrono::system_clock::now()}; }

    void start() override ;

    void stop() override ;

private:
    void loop() ;

    MessageBus& bus_;
    std::unique_ptr<ILinBus> hal_;
    std::string channel_;
    int baudrate_;
    LinScheduler scheduler_;
    LinNormalizer normalizer_;
    int rx_timeout_ms_;
    std::atomic<bool> running_{false};
    std::thread worker_;
    std::unique_ptr<ILinDriver> driver;
};

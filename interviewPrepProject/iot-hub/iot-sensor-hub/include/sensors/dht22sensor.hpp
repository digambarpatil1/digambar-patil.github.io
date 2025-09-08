#pragma once
#include "sensors/ISensor.hpp"
#include "core/messagebus.hpp"

#include <random>
#include <thread>
#include <iostream>
#include <string>   
#include <atomic>
class DHT22Sensor : public ISensor {
private:
    MessageBus& msgBus; // Reference to the message bus for publishing data
    int intervalMs; // Sampling interval in milliseconds
    std::atomic<bool> running;  // Control flag for the sensor thread
    std::thread sensorThread;

public:
        DHT22Sensor(MessageBus& bus, int interval_ms = 2000);
        void start() override;
        void stop() override;
        ~DHT22Sensor() override;
            
};  
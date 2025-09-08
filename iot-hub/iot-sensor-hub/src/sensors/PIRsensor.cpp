#include "sensors/PIRsensor.hpp"
#include <thread>
#include <chrono>
#include <random>
#include <nlohmann/json.hpp>

PIRsensor::PIRsensor(MessageBus& bus, int interval_ms)
    : msgBus(bus), intervalMs(interval_ms), running(false) {}

void PIRsensor::start() {
    running = true;
    sensorThread = std::thread([this]() {
        std::random_device rd;
        std::mt19937 gen(rd());
       static std::bernoulli_distribution motion(0.1); // 10% chance motion detected
        while (running) {
            nlohmann::json data = {
                {"motion", motion(gen)},
            };
            msgBus.publish(Message("PIR_1", data));
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
    });
}

void PIRsensor::stop() {
    running = false;
    if (sensorThread.joinable()) sensorThread.join();
}
PIRsensor::~PIRsensor() {
    stop();
}
#include "sensors/dht22sensor.hpp"
#include <thread>
#include <chrono>
#include <random>
#include <nlohmann/json.hpp>

DHT22Sensor::DHT22Sensor(MessageBus& bus, int interval_ms)
    : msgBus(bus), intervalMs(interval_ms), running(false) {}

void DHT22Sensor::start() {
    running = true;
  
    sensorThread = std::thread([this]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> temp_dist(20.0, 30.0);
        std::uniform_real_distribution<> hum_dist(30.0, 70.0);
        try{
            while (running) {
                nlohmann::json data = {
                    {"temperature", temp_dist(gen)},
                    {"humidity", hum_dist(gen)}
                };
                msgBus.publish(Message("DHT22_1", "test", data));
                std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            }
        } catch (const std::exception &e) {
            std::cerr << "Thread exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception in thread" << std::endl;
        }
    });
}

void DHT22Sensor::stop() {
    running = false;
    if (sensorThread.joinable()) sensorThread.join();
}
DHT22Sensor::~DHT22Sensor() {
    stop();
}
#pragma once
#include "ILinDriver.hpp"
#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>
#include <random>

class SimLinDriver : public ILinDriver {
public:
    SimLinDriver() : running_(false) {}
    ~SimLinDriver() override { stop(); }

    bool open(const std::string& channel, int baudrate) override {
        running_ = true;
        worker_ = std::thread(&SimLinDriver::generateFrames, this);
        return true;
    }

    void close() override {
        stop();
    }

    bool send(const std::vector<uint8_t>& frame) override {
        std::lock_guard<std::mutex> lock(mutex_);
        frameQueue_.push(frame); // optional: keep it if you want sensor-sent frames
        return true;
    }

    bool receive(std::vector<uint8_t>& frame) override {
        std::lock_guard<std::mutex> lock(mutex_);
        if(frameQueue_.empty()) return false;
        frame = frameQueue_.front();
        frameQueue_.pop();
        return true;
    }

private:
    void stop() {
        running_ = false;
        if (worker_.joinable()) worker_.join();
        std::lock_guard<std::mutex> lock(mutex_);
        while(!frameQueue_.empty()) frameQueue_.pop();
    }

    void generateFrames() {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> idDist(16, 17);
        std::uniform_int_distribution<int> valDist(0, 255);

        while(running_) {
            std::vector<uint8_t> frame(8, 0); // 8-byte payload
            frame[0] = static_cast<uint8_t>(idDist(rng)); // frame ID
            for(int i = 1; i < 8; ++i) frame[i] = static_cast<uint8_t>(valDist(rng));

            {
                std::lock_guard<std::mutex> lock(mutex_);
                frameQueue_.push(frame);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // mimic bus timing
        }
    }

    std::queue<std::vector<uint8_t>> frameQueue_;
    std::mutex mutex_;
    std::thread worker_;
    std::atomic<bool> running_;
};

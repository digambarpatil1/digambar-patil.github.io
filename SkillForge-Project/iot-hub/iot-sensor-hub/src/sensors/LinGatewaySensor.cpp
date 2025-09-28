#include "lin/ILinBus.hpp"
#include "lin/LinSchedule.hpp"
#include "lin/LinNormalizer.hpp"
#include "lin/LinTypes.hpp"
#include "core/messagebus.hpp"
#include "logger/Logger.hpp"
#include "sensors/ISensor.hpp" // your ISensor with start/stop
#include "sensors/LinGatewaySensor.hpp" 
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include<iostream>


LinGatewaySensor::LinGatewaySensor(MessageBus& bus,
                     std::unique_ptr<ILinBus> hal,
                     std::string channel,
                     int baudrate,
                     LinScheduler scheduler,
                     LinNormalizer normalizer,
                     int rx_timeout_ms,
                     std::unique_ptr<ILinDriver> drv)
    : bus_(bus),
      hal_(std::move(hal)),
      channel_(std::move(channel)),
      baudrate_(baudrate),
      scheduler_(std::move(scheduler)),
      normalizer_(std::move(normalizer)),
      rx_timeout_ms_(rx_timeout_ms),
      driver(std::move(drv)){}

LinGatewaySensor::~LinGatewaySensor()  { stop(); }

std::string LinGatewaySensor::getSensorName() const  { return "LinGateway"; }

// No direct readData() in the threaded model; we publish from the loop.
// SensorData readData()  { return {"LIN", 0.0, std::chrono::system_clock::now()}; }

void LinGatewaySensor::start()  {
    if (running_) return;
    if (!hal_->init(channel_, baudrate_)) {
        Logger::instance().log_error("LIN HAL init failed");
        return;
    }
    running_ = true;
    worker_ = std::thread([this]{ loop(); });
}

void LinGatewaySensor::stop()  {
    running_ = false;
    if (worker_.joinable()) worker_.join();
    if (hal_) hal_->shutdown();
}


void LinGatewaySensor::loop() {
    Logger::instance().log_info("LinGatewaySensor started");
    while (running_) {
        auto now = std::chrono::steady_clock::now();
        auto due = scheduler_.dueFrames(now);
        for (auto id : due) {
           // if (!hal_->request(id)) continue;
            std::vector<uint8_t> payload;
            if(!(driver->requestFrame(id, payload))) {
                Logger::instance().log_warning("LIN frame request failed for id " + std::to_string(id));
                continue;
            }
        
            LinFrame f;
            f.id = id;
            f.dlc = static_cast<uint8_t>(std::min<size_t>(8, payload.size()));
            std::fill(f.data.begin(), f.data.end(), 0);
            for (size_t i = 0; i < f.dlc; ++i) f.data[i] = payload[i];
            f.ts = std::chrono::system_clock::now();

            auto json = normalizer_.normalize(f);
            // Publish each signal individually to its own topic: lin/<frame_id>/<signal_name>
           /// for (auto& [signal_name, value] : json.items()) {
           //     Message m{"LIN_Gateway", "lin/" + std::to_string(id) + "/" + signal_name, value};
           //     bus_.publish(m);
           // }
                nlohmann::json json_signals;
                if (json.contains("signals")) {
                    json_signals = json["signals"];
                }
                if (!json.empty()) {
                std::string topic = "lin/" + std::to_string(id) + "/signals";
                Message m{"LIN_Gateway",topic,json_signals};
                bus_.publish(m);
                Logger::instance().log_info(
                    "[LIN] Published to topic: " + topic +
                    " | payload: " + json_signals.dump()
                );
              //  std::cout << "[LIN] Published to topic: " << topic
               //           << " | payload: " << json_signals.dump() << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // avoid busy-wait
    }
    Logger::instance().log_info("LinGatewaySensor stopped");
}

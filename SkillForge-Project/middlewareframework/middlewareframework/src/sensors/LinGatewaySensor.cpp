#include "sensors/LinGatewaySensor.hpp"
#include "core/Logger.hpp"

/**
 * @brief Construct a new LinGatewaySensor object.
 *
 * Initializes LIN bus and scheduler for periodic frame polling.
 *
 * @param bus Reference to the application's message bus.
 * @param linBus Unique pointer to the LIN bus interface.
 * @param driver Unique pointer to a LIN driver (optional, for extensions).
 * @param scheduler LIN scheduler defining frame IDs and timing.
 * @param normalizer LinNormalizer to convert frames to JSON signals.
 * @param poll_interval_ms Polling interval in milliseconds.
 */
LinGatewaySensor::LinGatewaySensor(MessageBus& bus,
                                   std::unique_ptr<ILinBus> linBus,
                                   std::unique_ptr<ILinDriver> driver,
                                   LinScheduler scheduler,
                                   LinNormalizer normalizer,
                                   int poll_interval_ms)
    : bus_(bus),
      linBus_(std::move(linBus)),
      driver_(std::move(driver)),
      scheduler_(std::move(scheduler)),
      normalizer_(std::move(normalizer)),
      poll_interval_ms_(poll_interval_ms),
      running_(false)
{
    linBus_->init("sim", 19200); // Initialize LIN bus for simulation
}

/**
 * @brief Destroy the LinGatewaySensor object.
 *
 * Stops polling and joins the worker thread safely.
 */
LinGatewaySensor::~LinGatewaySensor() {
    stop();
}

/**
 * @brief Start the sensor polling thread.
 *
 * The polling thread iterates through the scheduler, reads frames,
 * normalizes them, and publishes messages to the message bus.
 */
void LinGatewaySensor::start() {
    // Logger::instance().log_info("Starting LIN sensor...");
    running_ = true;
    pollThread_ = std::thread(&LinGatewaySensor::pollingLoop, this);
}

/**
 * @brief Stop the sensor polling thread.
 *
 * Sets the running flag to false and joins the thread safely.
 */
void LinGatewaySensor::stop() {
    // Logger::instance().log_info("Stopping LIN sensor...");
    running_ = false;
    if (pollThread_.joinable()) pollThread_.join();
}

/**
 * @brief Polling loop that reads LIN frames and publishes normalized signals.
 *
 * Iterates over due frames according to the scheduler, reads payloads from
 * the LIN bus, normalizes them via LinNormalizer, and publishes JSON messages
 * on the message bus.
 */
void LinGatewaySensor::pollingLoop() {
    // Logger::instance().log_info("Starting LIN sensor polling loop...");

    // Track last publish timestamp per frame ID
    std::unordered_map<uint8_t, std::chrono::steady_clock::time_point> last_publish_;

    while (running_) {
        auto now = std::chrono::steady_clock::now();
        auto due_frames = scheduler_.dueFrames(now); // Frames that should be processed now

        for (auto id : due_frames) {
            int period_ms = scheduler_.getPeriodMs(id);

            // Skip frames if not enough time has passed
            if (last_publish_.count(id) &&
                std::chrono::duration_cast<std::chrono::milliseconds>(now - last_publish_[id]).count() < period_ms) {
                continue;
            }

            // Receive frame from LIN bus
            std::vector<uint8_t> payload;
            if (!linBus_->receiveFrame(id, payload)) {
                // Logger::instance().log_warning("LIN frame request failed for id " + std::to_string(id));
                continue;
            }

            // Prepare LinFrame
            LinFrame frame;
            frame.id = id;
            frame.dlc = static_cast<uint8_t>(std::min<size_t>(8, payload.size()));
            std::fill(frame.data.begin(), frame.data.end(), 0);
            for (size_t i = 0; i < frame.dlc; ++i) frame.data[i] = payload[i];
            frame.ts = std::chrono::system_clock::now();

            // Normalize frame signals
            auto json_signals = normalizer_.normalize(frame)["signals"];
            if (!json_signals.empty()) {
                std::string topic = "lin/" + std::to_string(id) + "/signals";
                Message msg{"LIN_Gateway", topic, json_signals};
                bus_.publish(msg);

                // Optional logging
                // Logger::instance().log_info("[LinGatewaySensor] Published: " + topic + " -> " + json_signals.dump());
            }

            // Update last publish timestamp
            last_publish_[id] = now;
        }

        // Sleep briefly to avoid busy-wait
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Logger::instance().log_info("Stopping LIN sensor polling loop...");
}

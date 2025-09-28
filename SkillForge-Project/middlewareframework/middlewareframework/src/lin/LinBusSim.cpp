#include "lin/LinBusSim.hpp"
#include "lin/driver/SimLinDriver.hpp"

/**
 * @brief Construct a new LinBusSim object.
 *
 * Initializes the LIN simulation bus and sets up
 * the simulated driver instance.
 */
LinBusSim::LinBusSim()
    : running_(false)
{
    driver_ = std::make_unique<SimLinDriver>();
}

/**
 * @brief Destroy the LinBusSim object.
 *
 * Ensures the polling thread is stopped and joined
 * before destruction.
 */
LinBusSim::~LinBusSim() {
    running_ = false;
    if (pollThread_.joinable()) pollThread_.join();
}

/**
 * @brief Initialize the simulated LIN bus.
 *
 * @param channel The virtual channel identifier.
 * @param baudrate The baudrate for simulation.
 * @return true if initialization succeeded, false otherwise.
 */
bool LinBusSim::init(const std::string& channel, int baudrate) {
    bool ok = driver_->open(channel, baudrate);
    return ok;
}

/**
 * @brief Send a LIN frame.
 *
 * Prepends the frame ID to the data for simulation purposes,
 * and forwards it to the driver.
 *
 * @param id LIN frame identifier.
 * @param data Frame payload data (without ID).
 * @return true if the frame was sent successfully.
 */
bool LinBusSim::sendFrame(uint8_t id, const std::vector<uint8_t>& data) {
    std::vector<uint8_t> frame = data;
    frame.insert(frame.begin(), id); // prepend ID for simulation
    return driver_->send(frame);
}

/**
 * @brief Receive a LIN frame from the simulated driver.
 *
 * The first byte of the frame is treated as the ID,
 * and the rest as the payload data.
 *
 * @param id Reference to store the received frame ID.
 * @param data Reference to store the received frame payload.
 * @return true if a frame was successfully received.
 */
bool LinBusSim::receiveFrame(uint8_t& id, std::vector<uint8_t>& data) {
    std::vector<uint8_t> frame;
    if (!driver_->receive(frame)) return false;
    if (frame.empty()) return false;

    id = frame[0];
    data.assign(frame.begin() + 1, frame.end());
    return true;
}

/**
 * @brief Start polling the LIN scheduler in a separate thread.
 *
 * The scheduler defines frame IDs and their periodicity.
 *
 * @param scheduler A configured LIN scheduler with frame schedule entries.
 */
void LinBusSim::poll(const LinScheduler& scheduler) {
    scheduler_ = scheduler;
    running_ = true;
    pollThread_ = std::thread(&LinBusSim::pollLoop, this);
}

/**
 * @brief Internal polling loop.
 *
 * Iterates through the scheduler entries, sleeping
 * according to each frame’s configured period.
 *
 * Note: Frame sending is currently a placeholder (commented out).
 */
void LinBusSim::pollLoop() {
    while (running_) {
        auto& entries = scheduler_.getSchedule();
        for (const auto& e : entries) {
            // TODO: Hook up frame sending here if needed.
            // sendFrame(e.id, e.data);
            std::this_thread::sleep_for(std::chrono::milliseconds(e.period_ms));
        }
    }
}

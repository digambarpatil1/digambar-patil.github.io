#pragma once
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

/**
 * @struct Message
 * @brief Represents a message that can be published on the MessageBus.
 *
 * Contains the sensor ID, topic, payload, and timestamp.
 * Used for inter-plugin and sensor communication via the MessageBus.
 */
struct Message
{
    std::string sensorid;   ///< Unique identifier of the sensor or sender
    std::string topic;      ///< Topic name for message categorization
    nlohmann::json payload; ///< Arbitrary JSON payload of the message
    std::chrono::system_clock::time_point timestamp; ///< Time the message was created

    /**
     * @brief Construct a new Message with sensor ID, topic, and payload.
     *
     * Timestamp is automatically set to current system time.
     *
     * @param id Sensor or sender ID.
     * @param tpc Topic name.
     * @param pl JSON payload.
     */
    Message(const std::string& id, const std::string& tpc, const nlohmann::json& pl)
        : sensorid(id), topic(tpc), payload(pl), timestamp(std::chrono::system_clock::now()) {}

    /**
     * @brief Default constructor.
     *
     * Initializes an empty message.
     */
    Message() = default;
};

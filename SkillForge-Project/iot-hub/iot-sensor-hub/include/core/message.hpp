#pragma once
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
struct Message
{
    std::string sensorid;
    nlohmann::json payload;
    std::chrono::system_clock::time_point timestamp;
    Message(const std::string& id, const nlohmann::json& pl)
        : sensorid(id), payload(pl), timestamp(std::chrono::system_clock::now()) {}
    Message() = default;
   
};

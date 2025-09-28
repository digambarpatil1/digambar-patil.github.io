#pragma once
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
struct Message
{
    std::string sensorid;
    std::string topic;
    nlohmann::json payload;
    std::chrono::system_clock::time_point timestamp;
    Message(const std::string& id,const std::string& tpc,const nlohmann::json& pl)
        : sensorid(id),topic(tpc), payload(pl), timestamp(std::chrono::system_clock::now()) {}
    Message() = default;
   
};

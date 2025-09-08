#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

class Config {
public:
    static nlohmann::json load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open config file: " + path);
        }
        nlohmann::json j;
        file >> j;
        return j;
    }
};

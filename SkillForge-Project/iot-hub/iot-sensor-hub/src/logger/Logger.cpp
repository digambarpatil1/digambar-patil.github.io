#include "logger/Logger.hpp"
#include <iostream>
#include <stdexcept>
#include <fmt/format.h>
#include <iomanip>
#include <sstream>


Logger &Logger::instance()
{
    static Logger instance;
        return instance;
}

void Logger::setLogFile(const std::string& fp) {

    std::lock_guard<std::mutex> lock(mtx);
    std::cout<<"logfile path"<<fp<<std::endl;
    filePath = fp;
    std::filesystem::path dir = std::filesystem::path(fp).parent_path();
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }   
    if (ofs.is_open()) {
        ofs.close();
    }
    ofs.open(fp, std::ios::app); // append mode
    if (!ofs) {
        std::cerr << "Logger: Failed to open log file: " << fp << std::endl;
    }
}


void Logger::log(LogLevel level, const std::string& msg) {

    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    ::localtime_r(&now_c, &tm_now);
    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    std::string formatted = fmt::format("[{}] [{}] {}", oss.str(), levelToStr(level), msg);
    ofs << formatted << std::endl;

    //std::cout << fmt::format("[{}] [{}] {}\n", oss.str(), levelToStr(level), msg);

    if(!ofs.is_open()) {
        std::cerr << "Logger: Log file not open!" << std::endl;
        return;
    }
    ofs.flush();


    if (ofs.tellp() >= static_cast<std::streampos>(maxSize)) {
        rotate();
    }
}

void Logger::log_debug(const std::string& msg) { log(LogLevel::DEBUG , msg); } 
void Logger::log_info(const std::string& msg) { log( LogLevel::INFO,msg); }
void Logger::log_warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
void Logger::log_error(const std::string& msg) { log(LogLevel::ERROR , msg); }

std::string Logger::levelToStr(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

// Simple log rotation: close current file, rename it, and open a new one
void Logger::rotate() {
    ofs.close();
    std::string rotatedFile = filePath + ".old";
    std::filesystem::rename(filePath, rotatedFile);
    ofs.open(filePath, std::ios_base::trunc);
    if (!ofs.is_open()) {
        throw std::runtime_error("Unable to open log file after rotation: " + filePath);
    }
}


#include "core/Logger.hpp"
#include <stdexcept>
#include <fmt/format.h>
#include <iomanip>
#include <sstream>
#include <filesystem>

/**
 * @brief Get the singleton instance of the Logger.
 * 
 * @return Logger& Reference to the singleton Logger.
 */
Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

/**
 * @brief Set the log file path and initialize file stream.
 * 
 * Creates parent directories if they do not exist. Opens the file in append mode.
 * 
 * @param fp Path to the log file.
 */
void Logger::setLogFile(const std::string& fp) {
    std::lock_guard<std::mutex> lock(mtx);
    filePath = fp;

    // Ensure directory exists
    std::filesystem::path dir = std::filesystem::path(fp).parent_path();
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }

    // Close old file if open
    if (ofs.is_open()) {
        ofs.close();
    }

    // Open log file in append mode
    ofs.open(fp, std::ios::app);
    if (!ofs) {
        throw std::runtime_error("Logger: Failed to open log file: " + fp);
    }
}

/**
 * @brief Write a log message with given severity level.
 * 
 * Automatically prepends timestamp and log level.  
 * Rotates the file if max size is exceeded.
 * 
 * @param level LogLevel (DEBUG, INFO, WARNING, ERROR).
 * @param msg Message to log.
 */
void Logger::log(LogLevel level, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);

    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    ::localtime_r(&now_c, &tm_now);

    // Format timestamp
    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");

    // Format final log line
    std::string formatted = fmt::format("[{}] [{}] {}", oss.str(), levelToStr(level), msg);

    if (!ofs.is_open()) {
        throw std::runtime_error("Logger: Log file not open!");
    }

    ofs << formatted << std::endl;
    ofs.flush();

    // Rotate log file if size exceeds maxSize
    if (ofs.tellp() >= static_cast<std::streampos>(maxSize)) {
        rotate();
    }
}

/**
 * @brief Log a debug-level message.
 * @param msg The message to log.
 */
void Logger::log_debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }

/**
 * @brief Log an info-level message.
 * @param msg The message to log.
 */
void Logger::log_info(const std::string& msg) { log(LogLevel::INFO, msg); }

/**
 * @brief Log a warning-level message.
 * @param msg The message to log.
 */
void Logger::log_warning(const std::string& msg) { log(LogLevel::WARNING, msg); }

/**
 * @brief Log an error-level message.
 * @param msg The message to log.
 */
void Logger::log_error(const std::string& msg) { log(LogLevel::ERROR, msg); }

/**
 * @brief Convert LogLevel enum to string.
 * 
 * @param level LogLevel enum.
 * @return std::string Human-readable string representation.
 */
std::string Logger::levelToStr(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
    }
    return "UNKNOWN";
}

/**
 * @brief Rotate the current log file.
 * 
 * Closes the active log file, renames it with `.old`, 
 * and opens a fresh file with the same name.
 * 
 * @throws std::runtime_error if unable to reopen log file.
 */
void Logger::rotate() {
    ofs.close();
    std::string rotatedFile = filePath + ".old";

    // Replace old rotated file if exists
    if (std::filesystem::exists(rotatedFile)) {
        std::filesystem::remove(rotatedFile);
    }

    std::filesystem::rename(filePath, rotatedFile);

    ofs.open(filePath, std::ios_base::trunc);
    if (!ofs.is_open()) {
        throw std::runtime_error("Unable to open log file after rotation: " + filePath);
    }
}

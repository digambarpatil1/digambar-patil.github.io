#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <filesystem>

/**
 * @class Logger
 * @brief Thread-safe singleton logger with file output and log rotation.
 *
 * Provides methods to log messages at different levels (DEBUG, INFO, WARNING, ERROR).
 * Supports setting a maximum log file size and automatically rotates logs when exceeded.
 */
class Logger {
public:
    /**
     * @enum LogLevel
     * @brief Defines the severity levels for logging.
     */
    enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

    /**
     * @brief Get the singleton instance of Logger.
     * 
     * @return Logger& Reference to the Logger instance.
     */
    static Logger& instance();

    /**
     * @brief Set the log file path.
     *
     * Creates directories if they do not exist and opens the file in append mode.
     *
     * @param fp Path to the log file.
     */
    void setLogFile(const std::string& fp);

    /**
     * @brief Log a message with the specified log level.
     *
     * Automatically rotates the log if the file exceeds maxSize.
     *
     * @param level Log level (DEBUG, INFO, WARNING, ERROR)
     * @param msg Message string to log
     */
    void log(LogLevel level, const std::string& msg);

    /// Convenience method to log a DEBUG message
    void log_debug(const std::string& msg);

    /// Convenience method to log an INFO message
    void log_info(const std::string& msg);

    /// Convenience method to log a WARNING message
    void log_warning(const std::string& msg);

    /// Convenience method to log an ERROR message
    void log_error(const std::string& msg);

    /**
     * @brief Set the maximum size of the log file before rotation.
     * 
     * @param size Maximum file size in bytes.
     */
    void setMaxSize(std::size_t size) { maxSize = size; }

private:
    std::string filePath = "logs/app.log"; ///< Default log file path
    std::ofstream ofs;                     ///< Output file stream
    std::mutex mtx;                        ///< Mutex for thread-safe logging
    size_t maxSize = 5 * 1024 * 1024;     ///< Maximum log file size (default 5 MB)

    /**
     * @brief Convert LogLevel enum to string representation.
     *
     * @param level Log level
     * @return std::string Level as string
     */
    std::string levelToStr(LogLevel level);

    /**
     * @brief Rotate the current log file when it exceeds maxSize.
     *
     * Closes the current log, renames it, and opens a new log file.
     */
    void rotate();

    /// Private constructor for singleton pattern
    Logger() = default;
};

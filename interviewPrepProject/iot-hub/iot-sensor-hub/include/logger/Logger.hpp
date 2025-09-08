#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <filesystem>



 
class Logger {
public:
    enum class LogLevel { DEBUG, INFO, WARNING, ERROR };
    static Logger& instance();
    void setLogFile(const std::string& fp);
    void log(LogLevel level,const std::string& msg);
    // Convenience methods
    void log_debug(const std::string& msg);   
    void log_info(const std::string& msg) ;
    void log_warning(const std::string& msg); 
    void log_error(const std::string& msg);
    void setMaxSize(std::size_t size) { maxSize = size; }

    private:
    std::string filePath = "logs/app.log"; // Default log file path
    std::ofstream ofs;
    std::mutex mtx;

    size_t maxSize = 5 * 1024 * 1024; // 5 MB default
    std::string levelToStr(LogLevel level);
    void rotate();
    Logger() = default;   // Private constructor for singleton
  

};

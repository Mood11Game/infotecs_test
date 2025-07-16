#include "loglib/logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace loglib {

    Logger::Logger(const std::string& filename, LogLevel defaultLevel)
        : currentLevel_(defaultLevel)
    {
        logFile_.open(filename, std::ios::app);
    }

    Logger::~Logger() 
    {
        if (logFile_.is_open()) 
        {
            logFile_.close();
        }
    }

    void Logger::log(const std::string& message, LogLevel level) 
    {
        if (level < currentLevel_) 
            return;

            
        

        std::lock_guard<std::mutex> lock(mutex_);
        if (logFile_.is_open()) 
        {
            logFile_ << "[" << getCurrentTime() << "] "
                    << "[" << levelToString(level) << "] "
                    << message << std::endl;
        }
    }

    void Logger::setLogLevel(LogLevel level) 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        currentLevel_ = level;
    }

    LogLevel Logger::getLogLevel() const 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return currentLevel_;
    }

    std::string Logger::getCurrentTime() const 
    {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now;
        localtime_r(&time_t_now, &tm_now);

        std::ostringstream oss;
        oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string Logger::levelToString(LogLevel level) const 
    {
        switch (level) 
        {
            case LogLevel::Error: return "ERROR";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Info: return "INFO";
            default: return "UNKNOWN";
        }
    }

} 

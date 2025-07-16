#pragma once

#include "ilogger.hpp"  
#include <fstream>
#include <mutex>

namespace loglib
{
    class Logger : public ILogger
    {
    public:
        Logger(const std::string& fileName = "file_in_write.txt", LogLevel defaultLevel= LogLevel::Info);
        ~Logger();

        void log(const std::string& message, LogLevel level) override;
        void setLogLevel(LogLevel level) override;
        LogLevel getLogLevel() const override;

    private:
        mutable std::mutex mutex_;
        std::ofstream logFile_;
        LogLevel currentLevel_;

        std::string getCurrentTime() const;
        std::string levelToString(LogLevel level) const;
    };
}

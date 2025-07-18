#pragma once

#include "ilogger.hpp"  
#include <fstream>
#include <mutex>
#include <thread>
#include <iostream>

namespace loglib
{
    class Logger : public ILogger
    {
    public:
        Logger(const std::string& fileName, LogLevel defaultLevel);
        Logger();
        ~Logger();

        const std::string& getLogFileName() const;
        void setLogFile(const std::string& newFileName);
        void log(const std::string& message);
        
        void log(const std::string& message, LogLevel level) override;
        void setLogLevel(LogLevel level) override;
        LogLevel getLogLevel() const override;

    private:
        mutable std::mutex mutex_;
        std::ofstream logFile_;
        std::string logNameFile_;
        LogLevel currentLevel_;

        std::string getCurrentTime() const;
        std::string levelToString(LogLevel level) const;
        void initLogFile();
    };
}

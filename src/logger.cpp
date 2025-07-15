#include <loglib/logger.hpp>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace loglib
{
    Logger::Logger(const std::string& filename, LogLevel defaultLevel)
        : currentLevel_(defaultLevel)
    {
        logFile_.open(filename, std::ios::app);
    }

    Logger::~Logger()
    {
        if (logFile_.is_open())
            logFile_.close();
    }



}
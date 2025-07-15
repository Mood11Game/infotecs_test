#pragma once

#include <string>
#include "log_level.hpp"

namespace loglib
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void log(const std::string& message, LogLevel level) = 0;
        virtual void setLogLevel(LogLevel level) = 0;
        virtual LogLevel getLogLevel() const = 0;
    };
}
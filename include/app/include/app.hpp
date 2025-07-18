#pragma once

#include "loglib/logger.hpp"
#include <utility>
#include <queue>
#include <string>
#include <optional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace app
{
    class App
    {
    public:
        explicit App(std::optional<std::string> logNameFile = std::nullopt, 
            std::optional<loglib::LogLevel> defaultLevel = std::nullopt);
        ~App();

        void run();

    private:
        void startMess() const;
        void workerTheradFunc();
        void stop();
        
        std::queue<std::pair<std::string, loglib::LogLevel>> messageQueue_;
        std::mutex queueMutex_;
        std::condition_variable queueCV_;
        std::atomic<bool> running_;

        std::thread workerThread_;

        loglib::Logger logger_;
    };
}

#include "app/include/app.hpp"
#include <sstream>
#include <iostream>

namespace app
{
    App::App(std::optional<std::string> logNameFile, 
            std::optional<loglib::LogLevel> defaultLevel)
    :   logger_(), running_(true)
    {
        if (logNameFile.has_value() && defaultLevel.has_value())
        {
            logger_.setLogFile(logNameFile.value());
            logger_.setLogLevel(defaultLevel.value());
        }
        
        workerThread_ = std::thread(&App::workerTheradFunc, this);
    }

    App::~App()
    {
        stop();
        if (workerThread_.joinable())
        {
            workerThread_.join();
        }
    }

    void App::stop()
    {
        running_ = false;
        queueCV_.notify_all();
    }

    void App::workerTheradFunc()
    {
        while(running_ || !messageQueue_.empty())
        {
            std::unique_lock<std::mutex> ulock(queueMutex_);
            queueCV_.wait(ulock, [&]
                {
                    return !messageQueue_.empty() || !running_;
                });


            while(!messageQueue_.empty())
            {
                auto [mess, level] = messageQueue_.front();
                messageQueue_.pop();

                ulock.unlock();
                logger_.log(mess, level);
                ulock.lock();

            }
            
        }    
    }
    
    void App::startMess() const
    {
        std::cout << "Enter message. Fromat: [LEVEL] message\n"
                << "LEVEL: INFO, WARNING, ERROR(optional default INFO)\n"
                << "Type 'exit' to quit\n";
        
    }

    void App::run()
    {
        startMess();

        std::string inputLine;
        while(true)
        {
            std::getline(std::cin, inputLine);

            if (inputLine == "exit")
            {
                stop();
                break;
            }

            std::string mess;
            loglib::LogLevel currnetLevel = logger_.getLogLevel();

            if(inputLine.rfind("INFO", 0) == 0)
            {
                currnetLevel = loglib::LogLevel::Info;
                mess = inputLine.substr(5);
            }
            else if(inputLine.rfind("WARNING", 0) == 0)
            {
                currnetLevel = loglib::LogLevel::Warning;
                mess = inputLine.substr(8);
            }
            else if(inputLine.rfind("ERROR", 0) == 0)
            {
                currnetLevel = loglib::LogLevel::Error;
                mess = inputLine.substr(6);
            }
            else
                mess = inputLine;


            std::lock_guard<std::mutex> lock(queueMutex_);
            messageQueue_.push({mess, currnetLevel});
        }
    }

}

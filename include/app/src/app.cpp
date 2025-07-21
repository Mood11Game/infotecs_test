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
        std::cout << "\n=== App logger ===\n" 
                << "- Enter message. Fromat: [LEVEL] message \n"
                << "- LEVEL: INFO, WARNING, ERROR(optional default INFO) \n"
                << "- Type 'setting' \n"
                << "- Type 'exit' to quit \n";
        
    }

    void App::settingMess() const
    {
        std::cout << "\n=== Setting ===\n" 
                << "1 - Show current log file name\n"
                << "2 - Change log file name\n"
                << "3 - Show current log level\n"
                << "4 - Change log level\n"
                << "5 - Return to main menu\n";

    }

    void App::setting()
    {
        int command;

        do
        {
            std::cout << "Enter command number: ";
            std::cin >> command;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

            switch (command)
            {
            case 1:
                std::cout << "Current log file: " << logger_.getLogFileName() << std::endl;
                break;
            case 2:
            {
                std::cout << "Current file: " << logger_.getLogFileName() << std::endl;
                std::cout << "Enter new log file name: ";
                std::string newFileNameH;
                std::getline(std::cin, newFileNameH);
                logger_.setLogFile(newFileNameH);
                break;
            }
            case 3:
                std::cout << "LEVEL: 0 - INFO, 1 - WARNING, 2 - ERROR\n";
                std::cout << "Current level: " << static_cast<int>(logger_.getLogLevel()) << std::endl;
                break;
            case 4:
            {
                std::cout << "LEVEL: 0 - INFO, 1 - WARNING, 2 - ERROR\n";
                std::cout << "Current level: " << static_cast<int>(logger_.getLogLevel()) << std::endl;
                std::cout << "Enter new level: ";
                int nl;
                std::cin >> nl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                logger_.setLogLevel(static_cast<loglib::LogLevel>(nl));
                break;
            }
            case 5:
                std::cout << "Returning to main menu...\n";
                break;
            default:
                std::cout << "Enter pleace 1 - 5\n";
                break;
            }    
        } while (command != 5);

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
            else if (inputLine == "setting")
            {
                settingMess();
                setting();
                startMess();
                continue;
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

            {    
                std::lock_guard<std::mutex> lock(queueMutex_);
                messageQueue_.push({mess, currnetLevel});
            }
            queueCV_.notify_one();

        }
    }
}

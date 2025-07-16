#include <iostream>
#include "loglib/logger.hpp"


int main()
{
    loglib::Logger logger;
    std::cout << "Current log status: "
        << static_cast<int>(logger.getLogLevel()) << std::endl;

    logger.log("It info message(write)", loglib::LogLevel::Info);
    logger.log("It warning(write)", loglib::LogLevel::Warning);
    logger.log("It error(write)", loglib::LogLevel::Error);

    std::cout << "Change level log(Error)\n";
    logger.setLogLevel(loglib::LogLevel::Error);
    std::cout << "Current log status: "
        << static_cast<int>(logger.getLogLevel()) << std::endl;

    logger.log("It info message(not writing)", loglib::LogLevel::Info);
    logger.log("It warning(not writing)", loglib::LogLevel::Warning);
    logger.log("It error(write)", loglib::LogLevel::Error);

    std::cout << "The end -) \n";
    
}
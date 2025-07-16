#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include "loglib/logger.hpp"

void check_file_exists(const std::string& filename) 
{
    std::ifstream file(filename);
    assert(file.is_open() && "Log file was not created or cannot be opened.");
    file.close();
}

int check_log_content(const std::string& filename) 
{
    std::ifstream file(filename);
    assert(file.is_open() && "Cannot open log file for reading.");

    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) 
    {
        std::cout << line << std::endl;
        ++line_count;
        assert(
            line.find("INFO") != std::string::npos ||
            line.find("WARNING") != std::string::npos ||
            line.find("ERROR") != std::string::npos
        );
    }

    file.close();
    return line_count;
}

void check_log_level_change(loglib::Logger& logger, const std::string& filename, int previous_line_count) 
{
    logger.setLogLevel(loglib::LogLevel::Error);
    logger.log("Should not appear", loglib::LogLevel::Info);
    logger.log("Should appear", loglib::LogLevel::Error);

    std::ifstream file(filename);
    assert(file.is_open() && "Cannot open log file for reading after log level change.");

    std::string line;
    int new_line_count = 0;
    while (std::getline(file, line)) 
    {
        ++new_line_count;
    }
    file.close();

    assert(new_line_count == previous_line_count + 1 && "Incorrect number of lines after logging with new level.");
}

int main() 
{
    const std::string filename = "test_log.txt";

    std::remove(filename.c_str());

    loglib::Logger logger(filename, loglib::LogLevel::Info);

    logger.log("This is an info message", loglib::LogLevel::Info);
    logger.log("This is a warning message", loglib::LogLevel::Warning);
    logger.log("This is an error message", loglib::LogLevel::Error);

    check_file_exists(filename);

    int line_count = check_log_content(filename);

    assert(line_count == 3 && "Expected exactly 3 log entries initially.");

    check_log_level_change(logger, filename, line_count);

    std::cout << "\n All tests passed successfully!\n";
}

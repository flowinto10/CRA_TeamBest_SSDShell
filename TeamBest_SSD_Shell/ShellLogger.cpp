#include "ShellLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

ShellLogger& ShellLogger::getInstance() {
    static ShellLogger instance;
    return instance;
}

void ShellLogger::log(const std::string& className,
    const std::string& functionName,
    const std::string& message)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm time_info;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&time_info, &now_time_t);
#else
    localtime_r(&now_time_t, &time_info);
#endif

    std::ostringstream datetimeStream;
    datetimeStream << std::setfill('0')
        << std::setw(2) << (time_info.tm_year % 100) << "."
        << std::setw(2) << (time_info.tm_mon + 1) << "."
        << std::setw(2) << time_info.tm_mday << " "
        << std::setw(2) << time_info.tm_hour << ":"
        << std::setw(2) << time_info.tm_min;

    std::ostringstream headerStream;
    headerStream << "[" << datetimeStream.str() << "] "
        << className << "." << functionName << "()";

    std::string header = headerStream.str();
    header.resize(40, ' ');

    std::cout << header << ": " << message << std::endl;
}
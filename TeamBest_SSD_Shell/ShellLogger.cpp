#include "ShellLogger.h"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

ShellLogger::ShellLogger() : logDirectory("logs"), logFileName("latest.txt") {
    if (!fs::exists(logDirectory)) {
        fs::create_directory(logDirectory);
    }
    logFile.open(logDirectory + "/" + logFileName, std::ios::app);
}

ShellLogger::~ShellLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

ShellLogger& ShellLogger::getInstance() {
    static ShellLogger instance;
    return instance;
}

void ShellLogger::rotateLogFileIfNeeded() {
    std::string fullPath = logDirectory + "/" + logFileName;
    if (fs::exists(fullPath) && fs::file_size(fullPath) >= MAX_LOG_FILE_SIZE) {
        logFile.close();

        std::ostringstream backupName;
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm time_info;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&time_info, &now_time_t);
#else
        localtime_r(&now_time_t, &time_info);
#endif
        backupName << logDirectory << "/until_"
            << std::setfill('0')
            << std::setw(2) << (time_info.tm_year % 100)
            << std::setw(2) << (time_info.tm_mon + 1)
            << std::setw(2) << time_info.tm_mday << "_"
            << std::setw(2) << time_info.tm_hour
            << std::setw(2) << time_info.tm_min
            << std::setw(2) << time_info.tm_sec
            << ".log";

        fs::rename(fullPath, backupName.str());
        logFile.open(fullPath, std::ios::trunc);
    }
}

void ShellLogger::log(const std::string& fullFunctionName,
    const std::string& message)
{
    rotateLogFileIfNeeded();

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
        << fullFunctionName;

    std::string header = headerStream.str();
    header.resize(40, ' ');

    std::string fullLog = header + ": " + message + "\n";

    if (logFile.is_open()) {
        logFile << fullLog;
        logFile.flush();
    }
}

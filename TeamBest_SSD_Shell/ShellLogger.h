#pragma once

#include <string>
#include <fstream>

#define LOG_MESSAGE(cls, msg) \
    ShellLogger::getInstance().log(std::string(cls) + "." + __FUNCTION__, msg)

class ShellLogger {
public:
    static ShellLogger& getInstance();
    void log(const std::string& fullFunctionName,
        const std::string& message);

private:
    ShellLogger();
    ~ShellLogger();

    void rotateLogFileIfNeeded();
    void compressOldestLogFileIfNeeded();

    std::ofstream logFile;
    std::string logDirectory;
    std::string logFileName;
    static constexpr std::size_t MAX_LOG_FILE_SIZE = 10 * 1024; // 10KB

    ShellLogger(const ShellLogger&) = delete;
    ShellLogger& operator=(const ShellLogger&) = delete;
};

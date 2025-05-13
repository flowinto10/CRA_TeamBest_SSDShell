#pragma once

#include <string>
#include <fstream>

class ShellLogger {
public:
    static ShellLogger& getInstance();
    void log(const std::string& fullFunctionName,
        const std::string& message);

private:
    ShellLogger();
    ~ShellLogger();

    void rotateLogFileIfNeeded();

    std::ofstream logFile;
    std::string logDirectory;
    std::string logFileName;
    static constexpr std::size_t MAX_LOG_FILE_SIZE = 10 * 1024; // 10KB

    ShellLogger(const ShellLogger&) = delete;
    ShellLogger& operator=(const ShellLogger&) = delete;
};
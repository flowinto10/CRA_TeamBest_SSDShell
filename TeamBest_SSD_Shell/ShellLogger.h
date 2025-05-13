// ShellLogger.h
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

    std::ofstream logFile;
    std::string logDirectory;
    std::string logFileName;

    ShellLogger(const ShellLogger&) = delete;
    ShellLogger& operator=(const ShellLogger&) = delete;
};
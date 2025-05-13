#pragma once

#include <string>

class ShellLogger {
public:
    static ShellLogger& getInstance();
    void log(const std::string& className,
        const std::string& functionName,
        const std::string& message);

private:
    ShellLogger() = default;
    ~ShellLogger() = default;

    ShellLogger(const ShellLogger&) = delete;
    ShellLogger& operator=(const ShellLogger&) = delete;
};
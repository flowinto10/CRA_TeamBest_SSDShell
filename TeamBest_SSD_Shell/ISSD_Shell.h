#pragma once

#include <iostream>

class IShell {
public:
    virtual void PrintHelp() = 0;
    virtual std::string ReadInputFile(int address) = 0; // File Read
    virtual bool WriteOutputFile(std::string output, int address) = 0;  // File Write
    virtual void ProcessInputCommand(std::string command) = 0;  // Command Input Processing
    virtual bool ProcessParseInvalid(std::string command) = 0;  // Command Parsing & invalid 처리
};

#include "ISSD_Shell.h"

class SSDShell : public IShell{
public:
    void PrintHelp() override {}
    std::string ReadInputFile(int address) override { return 0x0; }
    bool WriteOutputFile(std::string output, int address) override { return true; }
    void ProcessInputCommand(std::string command) override {}
    bool ProcessParseInvalid(std::string command) override { return true; }
};
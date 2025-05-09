#include "ISSD_Shell.h"
#include <iostream>

using namespace std;

class SSDShell : public IShell{
public:

    void Run(void) {
        int loopCount = 0;
        std::string line;
        while (loopCount < 5) {
            // 1. print command cursor
            std::cout << "Shell>";

            // 2. input command
         // TODO : Enable later with Exit
         //   std::getline(std::cin, line);
         //   std::istringstream iss(line);

            // 3. parsing command
            // 3-1. invalid check
            // 4. process command

            //99. test code for loop count
            cout << "loop count :" << loopCount++ << endl;
        }
    }

    void PrintHelp() override {}
    std::string ReadInputFile(int address) override { return 0x0; }
    bool WriteOutputFile(std::string output, int address) override { return true; }
    void ProcessInputCommand(std::string command) override {}
    bool ProcessParseInvalid(std::string command) override { return true; }
};
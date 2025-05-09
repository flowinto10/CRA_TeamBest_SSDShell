#include "SSD_Shell.h"

void SSDShell::Run(void) {
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

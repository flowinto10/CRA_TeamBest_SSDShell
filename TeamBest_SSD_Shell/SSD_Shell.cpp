#include "ISSD_Shell.h"
#include <iostream>
using namespace std;

class SSDShell : public IShell{
public:
    void PrintHelp() override {
		cout << "SSD Shell Help" << endl;
		cout << "  command list : read, write, fullread, fullwrite, exit, help\n\n";
		cout << "  ---- usage ----" << endl;
		cout << "  read <lba 0~99>" << endl;
		cout << "  write <lba 0~99> <data>" << endl;
		cout << "        - data : 0~9, A-F, 4byte size" << endl;
		cout << "  fullread : read from 0 to 99" << endl;
		cout << "  fullwrite <data> : write from 0 to 99 with same data" << endl;
		cout << "        - data : 0~9, A-F, 4byte size" << endl;
		cout << "  exit" << endl;
		cout << "  help" << endl;
		cout << "  ----------------" << endl;
    }
    std::string ReadInputFile(int address) override { return 0x0; }
    bool WriteOutputFile(std::string output, int address) override { return true; }
    void ProcessInputCommand(std::string command) override {}
    bool ProcessParseInvalid(std::string command) override { return true; }
};
#pragma once
#include <iostream>
#include <string>
#include <istream>
#include <sstream>
#include "ISSD_Shell.h"

using namespace std;

class SSDShell {
public:
	enum class CommandType {
		Help = 0,
		Read = 1,
		Write = 2,
		Exit = 99,
	};
	struct SsdOp {
		std::string op;
		int address;
		int data;
	};

	void Run(void);
	void PrintHelp() {};
	std::string ReadSsdOutputFile(int address) { return 0x0; }
	SsdOp ParseInputArgument(std::string command) { return SsdOp{}; }
	void ExcuteCommand(std::string command) {}

private:
};
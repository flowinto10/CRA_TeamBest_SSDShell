#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream

#include <vector>
#include <regex>

#include "ShellCommandParser.h"
#include "ssdDriver.h"

using namespace std;

class SSDShell {
public:

	void Run(void);
	bool ExcuteCommand(ParsingResult command);

	std::string ReadSsdOutputFile(int address);
	bool WriteSsd(string data);
	bool EraseSsd(int lba, int size);
	bool EraseSsdRange(int start_lba, int end_lba);

	bool FullRead();
	bool FullWrite(string data);

	void PrintHelp();

	bool IsInvalidCommand();


private:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
	const int MIN_LBA = 0;
	const int MAX_LBA = 99;

	ShellCommandParser commandParser;
};
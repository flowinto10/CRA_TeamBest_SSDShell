#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream

#include <vector>
#include <regex>

#include "ssdDriver.h"

using namespace std;

enum Command {
	WRITE = 1,
	READ = 2,
	EXIT = 3,
	HELP = 4,
	FULL_WRITE = 5,
	FULL_READ = 6,
	SCRIPT_EXECUTE = 7,
	ERASE = 8,
	ERASE_RANGE =9,
	FLUSH = 10
};

enum InvalidType {
	NO_ERROR = 0,
	INVALID_COMMAND = 1,
	INVALID_DATA = 2,
	INVAILD_ADDRESS = 3,
	NO_INPUT_COMMAND = 4,
	NUMBER_OF_PARAMETERS_INCORRECT = 5
};


class SSDShell {
public:
	struct ParsingResult {
		int command;
		int startlba;
		int endlba_or_size;
		std::string data;
		std::string script_name;
		InvalidType invalidtype;
	} parsingresult;

	void Run(void);
	bool ExcuteCommand(ParsingResult command);

	bool ProcessParseInvalid(std::string command);
	bool IsInvalidCommand();
	std::string ReadSsdOutputFile(int address);
	bool WriteSsd(int address, string data);
	bool EraseSsd(int lba, int size);
	bool EraseSsdRange(int start_lba, int end_lba);

	bool FullRead();
	bool FullWrite(string data);

	void PrintHelp();

	int GetCommand(void) { return parsingresult.command; }
	int GetAddress(void) { return parsingresult.startlba; }
	std::string GetData(void) { return parsingresult.data; }
	InvalidType GetInvalidType(void) { return parsingresult.invalidtype; }

private:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
	const int MIN_LBA = 0;
	const int MAX_LBA = 99;

	vector<std::string> ParsingInputCommand(std::string command);
	void UpdateInvalidType_and_PrintErrorMessage(int error_type);
	bool UpdateCommand(std::string cmd);
	bool IsInvalidAddressRange(int lba);
};
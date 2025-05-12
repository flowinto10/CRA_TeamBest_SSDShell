#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream
#include "mock_ssd.cpp"

#include <vector>
#include <regex>

using namespace std;

enum Command {
	WRITE = 1,
	READ = 2,
	EXIT = 3,
	HELP = 4,
	FULL_WRITE = 5,
	FULL_READ = 6,
	SCRIPT_EXECUTE =7
};

enum InvalidType {
	NO_ERROR = 0,
	INVALID_COMMAND = 1,
	INVALID_DATA = 2,
	INVAILD_ADDRESS = 3,
	NO_INPUT_COMMAND = 4,
	NUMBER_OF_PARAMETERS_INCORRECT = 5
};

class SSDDriver {
public:
	void read(int address) {
		NiceMock<MockSSD> mock;
		mock.read(address);
		//--------------------------------------------------------------------------------------------------
		// todo : 나중에 실제로 구현된 ssd.exe 를 가지고 구현할 때는 이렇게?? 해야할 것 같아서 일단 남김.
		//int result = system("ssd.exe");

		//ostringstream commandStream;
		//commandStream << "ssd.exe " << address;
		//string command = commandStream.str();

		//// 2. 명령 실행
		//int result = system(command.c_str());
		//if (result != 0) {
		//	cerr << "ssd.exe 실행 실패. 종료 코드: " << result << endl;
		//	return;
		//}
	}
	void write(int address, string data) {
		NiceMock<MockSSD> mock;
		mock.write(address, data);
		//--------------------------------------------------------------------------------------------------
		// todo : 나중에 실제로 구현된 ssd.exe 를 가지고 구현할 때는 이렇게?? 해야할 것 같아서 일단 남김.
		//int result = system("ssd.exe");
		//ostringstream commandStream;
		//commandStream << "ssd.exe " << address << " " << data;
		//string command = commandStream.str();
		//// 2. 명령 실행
		//int result = system(command.c_str());
		//if (result != 0) {
		//	cerr << "ssd.exe 실행 실패. 종료 코드: " << result << endl;
		//	return;
		//}
	}

};

class SSDShell {
public:
	struct ParsingResult {
		int command;
		int script_number;
		int address;
		std::string data;
		InvalidType invalidtype;
	} parsingresult;

	void Run(void);
	bool ExcuteCommand(ParsingResult command);

	bool ProcessParseInvalid(std::string command);
	bool IsInvalidCommand();
	std::string ReadSsdOutputFile(int address);
	bool WriteSsd(string data);

	bool FullRead();
	bool FullWrite(string data);

	void PrintHelp();

	int GetCommand(void) { return parsingresult.command; }
	int GetAddress(void) { return parsingresult.address; }
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
	bool IsValidAddressRange();
};
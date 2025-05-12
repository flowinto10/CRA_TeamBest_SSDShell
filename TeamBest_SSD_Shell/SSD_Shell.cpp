#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream
#include "mock_ssd.cpp"

#include "ISSD_Shell.h"
#include <vector>
#include <regex>



enum Command {
	WRITE = 1,
	READ = 2,
	EXIT = 3,
	HELP = 4,
	FULL_WRITE = 5,
	FULL_READ = 6
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

	void PrintHelp() {
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


	string ReadSsdOutputFile(int address) {

		SSDDriver ssdDriver;
		ssdDriver.read(address);

		ifstream inputFile(SSD_OUTPUT);
		if (!inputFile) {
			cerr << "Error opening file for reading: " << SSD_OUTPUT << endl;
			return ""; //  todo. 에러처리에 대한 리턴을 어떻게 정의할지가 결정되면 업데이트 필요 
		}
		string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		inputFile.close();

		string result = "[Read] LBA " + to_string(address) + " : " + targetData;
		cout << result << endl;
		return result;
	}

	// full read 새로 구현
	bool FullRead() {
		SSDDriver ssdDriver;

		for (int address = MIN_LBA; address <= MAX_LBA; address++) {
			ssdDriver.read(address);
			ifstream inputFile(SSD_OUTPUT);
			
			if (!inputFile) {
				cout << "Error opening file for reading: " << SSD_OUTPUT << endl;
				return false; //  todo. 에러처리에 대한 리턴을 어떻게 정의할지가 결정되면 업데이트 필요 
			}
			string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
			string result = "[Read] LBA " + to_string(address) + " : " + targetData;
			cout << result << endl;
			inputFile.close();
		}
		
		return true;
	}

	// full write
	bool FullWrite(string data) {
		SSDDriver ssdDriver;
		for (int address = MIN_LBA; address <= MAX_LBA; address++) {
			ssdDriver.write(address, data);
		}
		return true;
	}

	bool ProcessParseInvalid(std::string command) {

		std::istringstream iss(command);
		std::string token;
		std::vector<std::string> tokens;

		while (iss >> token) {
			tokens.push_back(token);
		}

		if (tokens.empty()) {
			std::cout << "No Input command \n";
			parsingresult.invalidtype = NO_INPUT_COMMAND;
			return true;
		}

		std::string cmd = tokens[0];

		if (cmd == "write") {
			if (tokens.size() != 3) {
				std::cout << "The number of parameters are not correct  \n";
				parsingresult.invalidtype = NUMBER_OF_PARAMETERS_INCORRECT;
				return true;
			}
			try {
				int lba = std::stoi(tokens[1]);
				std::string value = tokens[2];

				// LBA Range Check
				if (lba < 0 || lba >= 100) {
					std::cout << "Invalid LBA Range \n";
					parsingresult.invalidtype = INVAILD_ADDRESS;
					return true;
				}

				// Value data range check
				if (!std::regex_match(value, std::regex("^0x[0-9A-Fa-f]{8}$"))) {
					std::cout << "Invalid Data \n";
					parsingresult.invalidtype = INVALID_DATA;
					return true;
				}

				parsingresult.command = WRITE;
				parsingresult.address = lba;
				parsingresult.data = value;

			}
			catch (...) {
				return true;
			}
			return false;
		}
		else if (cmd == "read") {
			if (tokens.size() != 2) { 
				std::cout << "The number of parameters are not correct  \n";
				parsingresult.invalidtype = NUMBER_OF_PARAMETERS_INCORRECT;
				return true; 
			}
			try {
				int lba = std::stoi(tokens[1]);
				if (lba < 0 || lba >= 100) {
					std::cout << "Invalid LBA Range \n";
					parsingresult.invalidtype = INVAILD_ADDRESS;
					return true;
				}

				parsingresult.command = READ;
				parsingresult.address = lba;
			}
			catch (...) {
				return true;
			}
			return false;
		}
		else if (cmd == "fullwrite") {
			if (tokens.size() != 2) {

				return true;
			}
			if (!std::regex_match(tokens[1], std::regex("^0x[0-9A-Fa-f]{8}$"))) {
				
				return true;
			}

			parsingresult.command = FULL_WRITE;
			parsingresult.data = tokens[1];

			return false;
		}
		else if (cmd == "fullread" || cmd == "exit" || cmd == "help") {
			if (cmd == "fullread")
				parsingresult.command = FULL_READ;
			else if (cmd == "exit")
				parsingresult.command = EXIT;
			else if (cmd == "help")
				parsingresult.command = HELP;

			return tokens.size() != 1;
		}

		std::cout << "Invalid Command \n";
		parsingresult.invalidtype = INVALID_COMMAND;
		return true;
	}

	int GetCommand(void) { return parsingresult.command; }
	int GetAddress(void) { return parsingresult.address; }
	std::string GetData(void) { return parsingresult.data; }
	int GetInvalidType(void) { return parsingresult.invalidtype; }

private:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
	const int MIN_LBA = 0;
	const int MAX_LBA = 99;

	struct ParsingResult {
		int command;
		int address;
		std::string data;
		int invalidtype;
	} parsingresult;
};
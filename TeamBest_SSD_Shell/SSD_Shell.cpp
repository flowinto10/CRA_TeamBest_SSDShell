#include "ISSD_Shell.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream
#include "mock_ssd.cpp"

using namespace std;

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

};

class SSDShell : public IShell {
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

	string ReadInputFile(int address) override {
		
		SSDDriver ssdDriver;
		ssdDriver.read(address);

		ifstream inputFile(SSD_NAND);
		string buffer;
		int lineNumber = -1;
		string targetData;
		while (getline(inputFile, buffer)) {
			++lineNumber;
			if (lineNumber == address) {
				targetData = buffer;
				break;
			}
		}
		inputFile.close();
		string result = "[Read] LBA " + to_string(address) + " : " + targetData;
		cout << result << endl;
		return result;
	}

	bool WriteOutputFile(std::string output, int address) override { return true; }
	void ProcessInputCommand(std::string command) override {}
	bool ProcessParseInvalid(std::string command) override { return true; }

private:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
};
#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

class ScriptCommand {
public:
	virtual void RunScript() = 0;
	virtual ~ScriptCommand() {}

	bool ReadCompare(int lba, string expectedData) {
		string exePath = "ssd.exe";
		string command = "";

		command = exePath + " r " + to_string(lba);
		int result = system(command.c_str());
		if (result != 0) {
			std::cerr << "Failed to execute command. Exit code: " << result << std::endl;
			return false;
		}

		ifstream inputFile(SSD_OUTPUT);
		if (!inputFile) {
			cerr << "Error opening file for reading: " << SSD_OUTPUT << endl;
			return false; //  todo. 에러처리에 대한 리턴을 어떻게 정의할지가 결정되면 업데이트 필요 
		}
		string actualData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		inputFile.close();

		actualData.erase(std::remove(actualData.begin(), actualData.end(), '\n'), actualData.end());

		if (actualData != expectedData)
		{
			string resultstr = "Fail : [Read Compare] LBA " + to_string(lba) + "  actual : " + actualData + ", expected : " + expectedData;
			cout << resultstr << endl;
			return false;
		}
		return true;
		
	}
private:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
};
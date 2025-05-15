#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "ShellLogger.h"
#include "ssdDriver.h"

using namespace std;

class ScriptCommand {
public:
	virtual bool RunScript() = 0;
	virtual ~ScriptCommand() {}

	SSDDriver* ssd = new SSDDriver();

	bool ReadCompare(int lba, string expectedData) {
		ssd->read(lba);

		ifstream inputFile(SSD_OUTPUT);
		if (!inputFile) {
			cerr << "Error opening file for reading: " << SSD_OUTPUT << endl;
			LOG_MESSAGE("Error opening file for reading: " + SSD_OUTPUT);
			return false;
		}
		string actualData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		inputFile.close();

		actualData.erase(std::remove(actualData.begin(), actualData.end(), '\n'), actualData.end());

		if (actualData != expectedData)
		{
			string resultstr = "Fail : [Read Compare] LBA " + to_string(lba) + "  actual : " + actualData + ", expected : " + expectedData;
			LOG_MESSAGE(resultstr);
			return false;
		}
		return true;
		
	}
private:
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
};
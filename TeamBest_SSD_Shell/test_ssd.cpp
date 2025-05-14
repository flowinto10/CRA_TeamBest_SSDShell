#pragma once

#include "gmock/gmock.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include "mock_ssd.cpp"
#include "ShellLogger.h"


using namespace std;
using namespace testing;

class MockSSDFixture : public Test {
public:
	MockSSD mock;
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름

	bool writeToFile(const string& filename, const string& data) {
		ofstream outputFile(filename);
		if (!outputFile) {
			cerr << "Error opening file for writing: " << filename << endl;
			LOG_MESSAGE("Error opening file for writing: " + filename);
			return false;
		}
		outputFile << data;
		outputFile.close();
		return true;
	}

	string readFromFile(const string& filename) {
		ifstream inputFile(filename);
		if (!inputFile) {
			cerr << "Error opening file for reading: " << filename << endl;
			LOG_MESSAGE("Error opening file for writing: " + filename);
			return "";
		}
		string data((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		inputFile.close();
		return data;
	}
};

TEST_F(MockSSDFixture, ReadTest_01) {
	MockSSD mock;
	int lba = 0;
	string expectedStr = "0x00000000";
	EXPECT_CALL(mock, read(lba)).Times(1); // read 는 리턴이 없으므로 호출횟수만 확인
	mock.read(lba);

	if (writeToFile(SSD_OUTPUT, expectedStr)) {
		string actualStr = readFromFile(SSD_OUTPUT);

		EXPECT_EQ(actualStr, expectedStr);
	}
	else {
		FAIL() << "Failed to write to file: " << SSD_OUTPUT;
	}
}

TEST_F(MockSSDFixture, WriteTest_01) {
	MockSSD mock;
	string expectedData = "0x11112222";
	int lba = 0;
	EXPECT_CALL(mock, write(lba, expectedData)).Times(1);
	mock.write(lba, expectedData);

	if (writeToFile(SSD_NAND, expectedData)) {
		string actualStr = readFromFile(SSD_NAND);

		EXPECT_EQ(actualStr, expectedData);
	}
	else {
		FAIL() << "Failed to write to file: " << SSD_NAND;
	}	
}
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ISSD_Shell.h"
#include "SSD_Shell.cpp"

using namespace std;
using namespace testing;

class ShellFixture : public Test {
public:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름

	bool writeToFile(const string& filename, const string& data) {
		ofstream outputFile(filename);
		if (!outputFile) {
			cerr << "Error opening file for writing: " << filename << endl;
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
			return "";
		}
		string data((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		inputFile.close();
		return data;
	}
};

TEST(ShellTest, PrintTC01) {
	SSDShell* shell = new SSDShell();
	shell->PrintHelp();
	EXPECT_TRUE(true); // 출력 되는것만 본다
}

TEST_F(ShellFixture, ReadSSDShell_tc01) {
	int lba = 0;
	string expectedStr = "0x00000000";

	// precondition
	fstream ssdNandFile(SSD_NAND);
	for (int i = 0; i < 100; ++i) {
		ssdNandFile << expectedStr << endl; // 100 줄을 모두 0x00000000으로 초기화
	}
	ssdNandFile.close();

	SSDShell* shell = new SSDShell();
	string actualStr = shell->ReadInputFile(lba);
	EXPECT_EQ(actualStr, "[Read] LBA " + to_string(lba) + " : " + expectedStr);
}
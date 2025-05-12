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
	bool ret = writeToFile(SSD_OUTPUT, "0x00000000"); // precondition. ssd.exe 에서 0x00000000 이라는 값을 읽었다고 치고
	if (ret == true) { // 파일 읽기가 성공하면
		string expectedStr = readFromFile(SSD_OUTPUT); //  shell 에서도 동일 값을 읽는지 체크함

		SSDShell* shell = new SSDShell();
		string actualStr = shell->ReadInputFile(lba);
		EXPECT_EQ(actualStr, "[Read] LBA " + to_string(lba) + " : " + expectedStr);
	}
	EXPECT_EQ(ret, true); // 파일 읽기 성공 여부 체크
}
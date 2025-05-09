#include "gmock/gmock.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include "mock_ssd.cpp"

using namespace std;
using namespace testing;

class MockSSDFixture : public Test {
public:
	MockSSD mock;
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND ���� �̸�
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD ��� ���� �̸�

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

TEST_F(MockSSDFixture, ReadTest_01) {
	MockSSD mock;
	int lba = 0;
	string expectedStr = "0x00000000";
	EXPECT_CALL(mock, read(lba)).Times(1); // read �� ������ �����Ƿ� ȣ��Ƚ���� Ȯ��
	mock.read(lba);

	writeToFile(SSD_OUTPUT, expectedStr);
	string actualStr = readFromFile(SSD_OUTPUT);

	EXPECT_EQ(actualStr, expectedStr);
}

TEST_F(MockSSDFixture, WriteTest_01) {
	MockSSD mock;
	string expectedData = "0x11112222";
	int lba = 0;
	EXPECT_CALL(mock, write(lba, expectedData)).Times(1);
	mock.write(lba, expectedData);

	writeToFile(SSD_NAND, expectedData);
	string actualStr = readFromFile(SSD_NAND);

	EXPECT_EQ(actualStr, expectedData);
}
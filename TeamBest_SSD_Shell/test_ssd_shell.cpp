#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SSD_Shell.h"

using namespace std;
using namespace testing;

class ShellFixture : public Test {
public:
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
	const string SSD_TEMP = "ssd_temp.txt"; // TC 에서 사용할 임시 파일 이름
	const int MIN_LBA = 0;
	const int MAX_LBA = 99;

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

	string readFromFile(const string& filename, int lba = 0) {
		ifstream inputFile(filename);
		if (!inputFile) {
			cerr << "Error opening file for reading: " << filename << endl;
			return "";
		}

		string data;
		int currentLba = 0;
		while (getline(inputFile, data)) {
			if (currentLba == lba) {
				break;
			}
			++currentLba;
		}

		inputFile.close();
		return data;
	}

	bool writeToFileWithLba(const string& filename, const string& data, int startlba, int endlba) {
		ofstream outputFile(filename);
		if (!outputFile) {
			cerr << "Error opening file for writing: " << filename << endl;
			return false;
		}
		for (int lba = startlba; lba <= endlba; lba++)
			outputFile << data << endl;
		outputFile.close();
		return true;
	}

	vector<string> readFromFileWithLba(const string& filename, int startlba, int endlba) {
		ifstream inputFile(filename);
		vector<string> result;
		if (!inputFile) {
			cerr << "Error opening file for reading: " << filename << endl;
			return result;
		}

		string line;
		int currentLba = 0;
		while (getline(inputFile, line)) {
			if (currentLba >= startlba && currentLba <= endlba) {
				result.push_back(line);
			}
			if (currentLba > endlba) break;
			++currentLba;
		}

		inputFile.close();
		return result;
	}

	bool compareFilesContents(const string& file1, const string& file2) {
		vector<string> lines1 = readFileToLines(file1);
		vector<string> lines2 = readFileToLines(file2);

		if (areLineVectorsEqual(lines1, lines2))
			return true;

		return false;
	}

private:
	vector<string> readFileToLines(const string& filename) {
		ifstream file(filename);
		vector<string> lines;
		if (!file) {
			cerr << "Error opening file: " << filename << endl;
			return lines;
		}

		string line;
		bool firstLine = true;
		while (getline(file, line)) {
			// 첫 줄의 BOM 제거
			if (firstLine && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
				line = line.substr(3);
			firstLine = false;

			// CR 제거 (Windows에서 생기는 문제)
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());

			lines.push_back(line);
		}

		return lines;
	}

	// vector<string> 비교 함수
	bool areLineVectorsEqual(const vector<string>& lines1, const vector<string>& lines2) {
		if (lines1.size() != lines2.size()) {
			cout << "Files differ in line count: " << lines1.size() << " vs " << lines2.size() << endl;
			return false;
		}

		for (size_t i = 0; i < lines1.size(); ++i) {
			if (lines1[i] != lines2[i]) {
				cout << "Difference at line " << i + 1 << ":\n";
				cout << "File1: " << lines1[i] << "\n";
				cout << "File2: " << lines2[i] << endl;
				return false;
			}
		}

		return true;
	}
};

TEST(ShellTest, PrintTC01) {
	SSDShell* shell = new SSDShell();
	shell->PrintHelp();
	EXPECT_TRUE(true); // 출력 되는것만 본다
}

TEST_F(ShellFixture, SSDShell_read_tc01) {
	int lba = 0;
	bool ret = writeToFile(SSD_OUTPUT, "0x00000000"); // precondition. ssd.exe 에서 0x00000000 이라는 값을 읽었다고 치고
	if (ret == true) { // 파일 읽기가 성공하면
		string expectedStr = readFromFile(SSD_OUTPUT); //  shell 에서도 동일 값을 읽는지 체크함

		SSDShell* shell = new SSDShell();
		string actualStr = shell->ReadSsdOutputFile(lba);
		EXPECT_EQ(actualStr, "[Read] LBA " + to_string(lba) + " : " + expectedStr);
	}
	EXPECT_EQ(ret, true); // 파일 write 성공 여부 체크
}


TEST_F(ShellFixture, SSDShell_FullWrite_tc01) {
	string dataPattern = "0xABCDABCD";

	// precondition
	bool ret = writeToFileWithLba(SSD_TEMP, dataPattern, MIN_LBA, MAX_LBA); // expected 용 임시파일 작성
	EXPECT_EQ(ret, true); // 파일 write 성공 여부 체크


	// ACT
	SSDShell* shell = new SSDShell();
	ret = shell->FullWrite(dataPattern); // SSD_NAND 파일에 write 한다고 가정
	EXPECT_EQ(ret, true);

	// todo 아직 ssd.exe 가 없으니까.. --------------
	ret = writeToFileWithLba(SSD_NAND, dataPattern, MIN_LBA, MAX_LBA); // todo 이거 나중에 ssd.exe 완성되면 삭제하자
	EXPECT_EQ(ret, true);
	//---------------------------------------

	// test
	ret = compareFilesContents(SSD_NAND, SSD_TEMP); // SSD_NAND 파일(actual)과 SSD_TEMP 파일(expected)이 동일한지 체크
	EXPECT_EQ(ret, true);
}

TEST_F(ShellFixture, SSDShell_FullRead_tc01) {
	string dataPattern = "0xABCDABCD";
	
	// ACT
	SSDShell* shell = new SSDShell();
	bool ret = shell->FullRead();
	

	// test
	EXPECT_EQ(ret, true);	// 함수 호출 성공 여부만 우선 확인
}

TEST_F(ShellFixture, SSDShell_FullRead_tc02) {
	string expectedDataPattern = "0xABCDABCD";

	// precondition
	writeToFile(SSD_OUTPUT, "0xABCDABCD"); // todo 아직 ssd.exe 가 없으니까...

	
	// ACT
	SSDShell* shell = new SSDShell();
	bool ret = shell->FullRead();


	// test
	EXPECT_EQ(ret, true); // full read 성공 여부 체크

	string actual = readFromFile(SSD_OUTPUT); // 마지막 read 한 데이터 패턴이 맞는지 비교해본다.
	EXPECT_EQ(actual, expectedDataPattern);

}

TEST_F(ShellFixture, SSDShell_FullRead_tc03) {
	// precondition
	writeToFile(SSD_OUTPUT, "0xABCDABCD"); // todo 아직 ssd.exe 가 없으니까 ssd_output.txt 에 write 해두고..

	// ACT
	SSDShell* shell = new SSDShell();
	bool ret = shell->FullRead();

	// test
	EXPECT_EQ(ret, true); // full read 성공 여부 체크

	// MIN_LBA 부터 MAX_LBA 까지 읽은 데이터가 맞는지 한땀씩 체크
	for (int i = MIN_LBA; i <= MAX_LBA; i++) {
		string actual = shell->ReadSsdOutputFile(i);
		string expected = "[Read] LBA " + to_string(i) + " : " + readFromFile(SSD_NAND, i);
		EXPECT_EQ(actual, expected);
	}
}
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

	//setup function 구현
	void SetUp() override {
		// SSD_NAND 파일을 초기화합니다.
		DeleteSSDNandFile();
	}

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

	void DeleteSSDNandFile() {
		const char* filename = "ssd_nand.txt";
		if (remove(filename) != 0) {
			std::perror("파일 삭제 실패");
		}
	}

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
	SSDShell* shell = new SSDShell();
	string actualStr = shell->ReadSsdOutputFile(lba);
	EXPECT_NE(actualStr, ""); // 파일 write 성공 여부 체크
}


TEST_F(ShellFixture, SSDShell_FullWrite_tc01) {
	string dataPattern = "0xABCDABCD";
	// ACT
	SSDShell* shell = new SSDShell();
	bool ret = shell->FullWrite(dataPattern); // SSD_NAND 파일에 write 한다고 가정
	EXPECT_EQ(ret, true);
}

TEST_F(ShellFixture, SSDShell_FullRead_tc01) {
	string dataPattern = "0xABCDABCD";

	// ACT
	SSDShell* shell = new SSDShell();
	bool ret = shell->FullWrite(dataPattern);
	EXPECT_TRUE(ret);
	ret = shell->FullRead();
	EXPECT_TRUE(ret);

	int lba = 99;
	string actualStr = shell->ReadSsdOutputFile(lba);
	EXPECT_NE(actualStr, ""); // 파일 write 성공 여부 체크
	string expectedStr = "[Read] LBA " + to_string(lba) + " : " + dataPattern;
	EXPECT_EQ(actualStr, expectedStr);

}
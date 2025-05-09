#include "ISSD_Shell.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system �Լ�
#include <sstream>  // ostringstream

using namespace std;

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
		// todo : ���߿� ������ ������ ssd.exe �� ������ ������ ���� �̷���?? �ؾ��� �� ���Ƽ� todo ����
		//int result = system("ssd.exe");

		//ostringstream commandStream;
		//commandStream << "ssd.exe " << address;
		//string command = commandStream.str();

		//// 2. ��� ����
		//int result = system(command.c_str());
		//if (result != 0) {
		//	cerr << "ssd.exe ���� ����. ���� �ڵ�: " << result << endl;
		//	return;
		//}


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
	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND ���� �̸�
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD ��� ���� �̸�
};
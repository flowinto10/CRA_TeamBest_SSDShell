//feature/erase_4_EraseAndWriteAging
// #pragma once
#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include "script_command.h"
#include "ShellLogger.h"
using namespace std;

/*

Test Scenario
• 0 ~ 2번 LBA 삭제
• Loop 30회
• 2번 LBA Write
• 2번 LBA OverWrite
• 2 ~ 4번 LBA 삭제
compare 하기 -> 0x00000000 기대
• 4번 LBA Write
• 4번 LBA OverWrite
• 4 ~ 6번 LBA 삭제
compare 하기
• 6번 LBA Write
• 6번 LBA OverWrite
• 6 ~ 8번 LBA 삭제
compare 하기

		*/

class EraseAndWriteAging : public ScriptCommand {
public:
	void RunScript() override {
		const string exePath = "ssd.exe";
		const string pattern = " 0xFFFFFFFF";
		const string erasePattern = "0x00000000";
		string command = "";
		for (int loop = 0; loop < 30; loop++) {
			for (int lba = 0; lba < 99; lba++) {
				if (lba + 2 > 99) 
					continue; // 99 이상의 lba 는 터치하지 않도록 제한

				for (int k = 0; k < 2; k++) { // 같은 lba 3개에 대해 2회 write
					for (int i = 0; i < 3; i++) {
						
						command = exePath + " w " + to_string(lba + i) + pattern;
						int result = system(command.c_str());
						if (result != 0) {
							std::cerr << "Failed to execute command. Exit code: " << result << std::endl;
							return;
						}
					}
				}
				
				command = exePath + " e " + to_string(lba) + " 3"; // lba 3개 erase
				int result = system(command.c_str());
				if (result != 0) {
					std::cerr << "Failed to execute command. Exit code: " << result << std::endl;
					return;
				}


				for (int i = 0; i < 3; i++) {
					ReadCompare(lba + i, erasePattern);
				}

				lba += 3; // 3개씩 증가


			}
		}


		string data[2] = { "0x11111111", "0xFFFFFFFF" };
		int start = 0;
		int idx = 0;
		string write = " w ";
		for (int i = 0; i < 100 / 5; i++) {
			start = i * 5;
			idx = getNext();
			string exePath = "ssd.exe";
			string command = "";
			for (int j = start; j < start + 5; j++) {
				command = exePath + write + to_string(j) + " " + data[idx];
				int result = system(command.c_str());
				if (result != 0) {
					std::cerr << "Failed to execute command. Exit code: " << result << std::endl;
					return;
				}
				LOG_MESSAGE("FullWriteAndReadCompare", "Write LBA " + to_string(j) + " with data: " + data[idx]);
			}

			for (int j = start; j < start + 5; j++) {
				bool ret = ReadCompare(j, data[idx]);
			}

			LOG_MESSAGE("EraseAndWriteAging", " Erase And Write Aging Success");
		}

	}
private:
	int getNext() {
		static int index = 0;
		int value = index % 2;
		index++;
		return value; // 0 or 1 만 리턴
	}
};


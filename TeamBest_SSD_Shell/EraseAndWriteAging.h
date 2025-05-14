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
	bool RunScript() override {
		bool ret = true;
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
							LOG_MESSAGE("Failed to execute command.Exit code : " + to_string(result));
							return false;
						}
					}
				}
				
				command = exePath + " e " + to_string(lba) + " 3"; // lba 3개 erase
				int result = system(command.c_str());
				if (result != 0) {
					std::cerr << "Failed to execute command. Exit code: " << result << std::endl;
					LOG_MESSAGE("Failed to execute command.Exit code : " + to_string(result));
					return false;
				}


				for (int i = 0; i < 3; i++) {
					ret = ReadCompare(lba + i, erasePattern);
					if (ret == false) return false;
				}
				lba += 3; // 3개씩 증가
			}
		}
		return true;
	}
};


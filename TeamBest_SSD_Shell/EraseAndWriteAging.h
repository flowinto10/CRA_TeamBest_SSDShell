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

		for (int lba = 0; lba <= 99; lba += 10) {
			ssd->erase(lba, 10); // tc 시작 전 format			
		}
		
		for (int loop = 0; loop < 30; loop++) {
			for (int lba = 0; lba <= 97; lba++) {
				if (lba + 2 > 99) 
					continue; // 99 이상의 lba 는 터치하지 않도록 제한

				for (int k = 0; k < 2; k++) { // 같은 lba 3개에 대해 2회 write
					for (int i = 0; i < 3; i++) {
						ssd->write(lba + i, pattern);
					}
				}
				
				ssd->erase(lba, 3);

				for (int i = 0; i < 3; i++) {
					if (lba + i > 99)
						continue;
					ret = ReadCompare(lba + i, erasePattern);
					if (ret == false) 
						return false;
				}

				lba += 3; // 3개씩 증가
			}
		}
		return true;
	}
};


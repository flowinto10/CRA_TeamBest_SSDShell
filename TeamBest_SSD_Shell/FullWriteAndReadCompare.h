#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include "script_command.h"
#include "ShellLogger.h"
using namespace std;

/*

0 ~ 4번 LBA까지 Write 명령어를 수행한다.
• 0 ~ 4번 LBA까지 ReadCompare 수행
• 5 ~ 9번 LBA까지 다른 값으로 Write 명령어를 수행한다.
• 5 ~ 9번 LBA까지 ReadCompare 수행
• 10 ~ 14번 LBA까지 다른 값으로 Write 명령어를 수행한다.
• 10 ~ 14번 LBA까지 ReadCompare 수행
• 위와 같은 규칙으로 전체 영역에 대해 Full Write + Read Compare를 수행한다

		*/

class FullWRiteAndReadCompare : public ScriptCommand {
public:
	bool RunScript() override {
		string data[2] = { "0x11111111", "0xFFFFFFFF" };
		int start = 0;
		int idx = 0;

		for (int lba = 0; lba <= 99; lba += 10) {
			ssd->erase(lba, 10); // tc 시작 전 format			
		}

		for (int i = 0; i < 100 / 5; i++) {
			start = i * 5;
			idx = getNext();
			
			for (int j = start; j < start + 5; j++) {
				ssd->write(j, data[idx]);
				LOG_MESSAGE("Write LBA " + to_string(j) + " with data: " + data[idx]);
			}

			for (int j = start; j < start + 5; j++) {
				bool ret = ReadCompare(j, data[idx]);
				if (ret == false) return false;
			}

			LOG_MESSAGE(" Write and Read Compare Success");
		}

		return true;

	}
private:
	int getNext() {
		static int index = 0;
		int value = index % 2;
		index++;
		return value; // 0 or 1 만 리턴
	}
};


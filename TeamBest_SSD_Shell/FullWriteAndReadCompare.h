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
	void RunScript() override {
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

			LOG_MESSAGE("FullWriteAndReadCompare"," Write and Read Compare Success");
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


#pragma once
#include "script_command.h"
#include <iostream>
#include "ShellLogger.h"

/*

• Loop 200회
• 0번 LBA에 랜덤 값을 적는다.
• 99번 LBA에 랜덤 값을 적는다.
• LBA 0번과 99번, ReadCompare를 수행

		*/

class WriteReadAging : public ScriptCommand {
public:
	bool RunScript() override {
		string pattern[2] = { "0xABCDABCD", "0x12340987" };
		int idx = 0;
		int lba = 0;

		for (int lba = 0; lba <= 99; lba += 10) {
			ssd->erase(lba, 10); // tc 시작 전 format			
		}

		for (int loop = 0; loop < 200; loop++) {
			ssd->write(lba, pattern[idx]);
			LOG_MESSAGE("Write LBA " + to_string(lba) + " with data: " + pattern[idx]);	
		}

		idx = 1;
		lba = 99;
		for (int loop = 0; loop < 200; loop++) {
			ssd->write(lba, pattern[idx]);
		}
		LOG_MESSAGE("Write LBA " + to_string(lba) + " with data: " + pattern[idx]);	

		bool ret = ReadCompare(0, pattern[0]);
		if (ret == false) 
			return false;
			
		ret = ReadCompare(99, pattern[1]);
		if (ret == false) 
			return false;

		LOG_MESSAGE("Write and Read Compare Success");
		return true;
	}
};

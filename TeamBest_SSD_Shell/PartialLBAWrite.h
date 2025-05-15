#pragma once
#include "script_command.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include "ShellLogger.h"

/*

Loop는 30회
• 4번 LBA에 값을 적는다.
• 0번 LBA에 같은 값을 적는다.
• 3번 LBA에 같은 값을 적는다.
• 1번 LBA에 같은 값을 적는다.
• 2번 LBA에 같은 값을 적는다.
• LBA 0 ~ 4번, ReadCompare

		*/

class PartialLBAWrite : public ScriptCommand {
public:
	bool RunScript() override {
		string pattern = "0xABCDABCD";
		int lbaIndex[5] = { 4,0,3,1,2 };
		int idx = 0;

		for (int lba = 0; lba <= 99; lba += 10) {
			ssd->erase(lba, 10); // tc 시작 전 format			
		}

		for (int loop = 0; loop < 30; loop++) {
			for (int lba = 0; lba < 5; lba++)
			{
				idx = lbaIndex[lba];
				ssd->write(idx, pattern);
				LOG_MESSAGE("Write LBA " + to_string(idx) + " with data: 0xABCDABCD");
			}
			for(int lba=0; lba < 5; lba++){
				bool ret = ReadCompare(idx, pattern);
				if (ret == false) 
					return false;
			}

			LOG_MESSAGE("Write and Read Compare Success");
		}
		return true;
	}
};


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
		
		string exePath = "ssd.exe";
		string command = "";

		int lbaIndex[5] = { 4,0,3,1,2 };
		int idx = 0;
		string write = " w ";
		string read = " r ";

		for (int loop = 0; loop < 30; loop++) {
			for (int lba = 0; lba < 5; lba++)
			{
				idx = lbaIndex[lba];
				command = exePath + write + to_string(idx) + " " + "0xABCDABCD";
				int result = system(command.c_str());
				if (result != 0) {
					std::cerr << "Failed to execute command. Exit code: " << result << std::endl;
					LOG_MESSAGE("Failed to execute command. Exit code : " + to_string(result));
					return false;
				}
				LOG_MESSAGE("Write LBA " + to_string(idx) + " with data: 0xABCDABCD");
			}
			for(int lba=0; lba < 5; lba++){
				bool ret = ReadCompare(idx, "0xABCDABCD");
				if (ret == false) return false;
			}

			LOG_MESSAGE("Write and Read Compare Success");
		}
		return true;
	}
};


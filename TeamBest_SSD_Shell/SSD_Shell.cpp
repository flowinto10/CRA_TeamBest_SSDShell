#include <cctype> // isspace
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream

#include <vector>
#include <regex>

#include "SSD_Shell.h"
#include "ShellLogger.h"
#include "script_executor.h"

void SSDShell::Run(void) {	
	std::string line;
#ifdef _DEBUG
	int loopCount = 0;
	while (loopCount < 5) {
#else
	while (true) {
#endif

		std::cout << "Shell>";

		std::getline(std::cin, line);
		std::istringstream iss(line);

		commandParser.ProcessParseInvalid(line);
		if (commandParser.GetParsingResult().IsInvalidCommand() == true)
		{
			continue;
		}

		ParsingResult parsingresult = commandParser.GetParsingResult();

		if (ExcuteCommand(parsingresult) == true) {
			break; // exit
		}
	}
}


void SSDShell::PrintHelp() {
	cout << ">>>> SSD Shell Help <<<<" << endl;
	cout << " * command list : read, write, fullread, fullwrite, erase, erase_range, exit, help, flush\n\n";
	cout << "  ---- usage ----" << endl;
	cout << "    read <lba 0~99>" << endl;
	cout << "    write <lba 0~99> <data>" << endl;
	cout << "        - data : 0~9, A-F, 4byte size" << endl;
	cout << "    fullread : read from 0 to 99" << endl;
	cout << "    fullwrite <data> : write from 0 to 99 with same data" << endl;
	cout << "        - data : 0~9, A-F, 4byte size" << endl;
	cout << "    erase : erase <lba 0~99> <size>" << endl;
	cout << "        - size : min int - max int " << endl;
	cout << "    erase_range : erase_range <start lba 0~99> <end lba 0~99>" << endl;
	cout << "    exit" << endl;
	cout << "    help" << endl;
	cout << "    flush" << endl;
	cout << "  ----------------" << endl;
	cout << ">>>> Test Shell Script Help <<<<" << endl;
	cout << "    <#>_<TC full name> : run script" << endl;
	cout << "    <#>_ : run script start with #" << endl;
	cout << "  ----------------" << endl;
	cout << "  made by team BEST " << endl;
	cout << "     박영조, 황영철, 김혜린, 김원석, 전은실" << endl;
	cout << "  ----------------" << endl;
}

string SSDShell::ReadSsdOutputFile(int address) {
	SSDDriver ssdDriver;
	ssdDriver.read(address);

	ifstream inputFile(SSD_OUTPUT);
	if (!inputFile) {
		cerr << "Error opening file for reading: " << SSD_OUTPUT << endl;
		LOG_MESSAGE("Error opening file for reading: " + SSD_OUTPUT);
		return "";
	}

	string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	inputFile.close();

	// 끝에 있는 \r, \n, 공백 문자 제거
	while (!targetData.empty() && isspace(static_cast<unsigned char>(targetData.back()))) {
		targetData.pop_back();
	}

	string result = "[Read] LBA " + to_string(address) + " : " + targetData;
	cout << result << endl;
	return targetData;
}

bool SSDShell::WriteSsd(int address, string data)
{
	SSDDriver ssdDriver;
	ssdDriver.write(address, data);
	cout << "[Write] LBA Done" << endl;
	return true;
}

bool SSDShell::FullRead() {
	SSDDriver ssdDriver;

	for (int address = MIN_LBA; address <= MAX_LBA; address++) {
		ssdDriver.read(address);
		ifstream inputFile(SSD_OUTPUT);

		if (!inputFile) {
			cout << "Error opening file for reading: " << SSD_OUTPUT << endl;
			return false; 
		}
		string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		string result = "[Read] LBA " + to_string(address) + " : " + targetData;
		cout << result << endl;
		inputFile.close();
	}

	return true;
}

bool SSDShell::FullWrite(string data) {
	SSDDriver ssdDriver;
	for (int address = MIN_LBA; address <= MAX_LBA; address++) {
		ssdDriver.write(address, data);
	}
	return true;
}

bool SSDShell::EraseSsd(int lba, int size) {
	SSDDriver ssdDriver;
	ssdDriver.erase(lba, size);
	return true;
}

bool SSDShell::EraseRangeToErase(int start_lba, int end_lba) {
	int lba = start_lba;
	int size = end_lba - start_lba + 1;
	SSDDriver ssdDriver;
	ssdDriver.erase(lba, size);
	return true;
}

bool SSDShell::Flush(void) {
	SSDDriver ssdDriver;
	ssdDriver.flush();
	return true;
}

bool SSDShell::ExcuteCommand(ParsingResult command) {
	bool ret = false;

	switch (command.GetCommand()) {
		case WRITE:
			WriteSsd(command.GetStartLba(), command.GetData());
			break;

		case READ:
			ReadSsdOutputFile(command.GetStartLba());
			break;

		case FULL_WRITE:
			FullWrite(command.GetData());
			break;

		case FULL_READ:
			FullRead();
			break;

		case HELP:
			PrintHelp();
			break;
		case EXIT:
			cout << "Exit" << endl;
			ret = true;
			break;
		case SCRIPT_EXECUTE:
		{
			cout << "Script Execute" << endl;
			ScriptExcutor* scriptExcutor = new ScriptExcutor();
			scriptExcutor->execute(command.GetScriptName());
			break;
		}
		case ERASE:
			cout << "ERASE " << command.GetStartLba() <<" "<< command.GetSize() <<endl;
			EraseSsd(command.GetStartLba(), command.GetSize());
			break;
		case ERASE_RANGE:
			cout << "ERASE RANGE" << command.GetStartLba() << " " << command.GetEndLba() << endl;
			EraseRangeToErase(command.GetStartLba(), command.GetEndLba());
			break;
		case FLUSH:
			cout << "FLUSH" << endl;
			Flush();
			break;

		default:
			break;
	}
	return ret;
}

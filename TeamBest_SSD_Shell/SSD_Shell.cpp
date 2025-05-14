#include <cctype> // isspace
#include "SSD_Shell.h"
#include "ShellLogger.h"
#include "script_executor.h"

void SSDShell::Run(void) {
	int loopCount = 0;
	std::string line;
#ifdef _DEBUG
	while (loopCount < 5) {
#else
	while (true) {
#endif
		//0. test code for loop count
		//cout << "loop count :" << loopCount++ << endl;

		// 1. print command cursor
		std::cout << "Shell>";

		// 2. input command
		// TODO : Enable later with Exit
		std::getline(std::cin, line);
		std::istringstream iss(line);

		// 3. parsing command
		commandParser.ProcessParseInvalid(line);
		// 3-1. invalid check
		if (commandParser.GetParsingResult().IsInvalidCommand() == true)
		{
			continue;
		}

		ParsingResult parsingresult = commandParser.GetParsingResult();

		// 4. process command
		if (ExcuteCommand(parsingresult) == true) {
			break; // exit
		}
	}
}


void SSDShell::PrintHelp() {
	cout << ">>>> SSD Shell Help <<<<" << endl;
	cout << " * command list : read, write, fullread, fullwrite, exit, help\n\n";
	cout << "  ---- usage ----" << endl;
	cout << "    read <lba 0~99>" << endl;
	cout << "    write <lba 0~99> <data>" << endl;
	cout << "        - data : 0~9, A-F, 4byte size" << endl;
	cout << "    fullread : read from 0 to 99" << endl;
	cout << "    fullwrite <data> : write from 0 to 99 with same data" << endl;
	cout << "        - data : 0~9, A-F, 4byte size" << endl;
	cout << "    exit" << endl;
	cout << "    help" << endl;
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
		return ""; //  todo. 에러처리에 대한 리턴을 어떻게 정의할지가 결정되면 업데이트 필요 
	}

	string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	inputFile.close();

	// 끝에 있는 \r, \n, 공백 문자 제거
	while (!targetData.empty() && isspace(static_cast<unsigned char>(targetData.back()))) {
		targetData.pop_back();
	}

	string result = "[Read] LBA " + to_string(address) + " : " + targetData;
	cout << result << endl;
	return result;
}

bool SSDShell::WriteSsd(int address, string data)
{
	SSDDriver ssdDriver;
	ssdDriver.write(address, data);
	cout << "[Write] LBA Done" << endl;
	return false;
}

// full read 새로 구현
bool SSDShell::FullRead() {
	SSDDriver ssdDriver;

	for (int address = MIN_LBA; address <= MAX_LBA; address++) {
		ssdDriver.read(address);
		ifstream inputFile(SSD_OUTPUT);

		if (!inputFile) {
			cout << "Error opening file for reading: " << SSD_OUTPUT << endl;
			return false; //  todo. 에러처리에 대한 리턴을 어떻게 정의할지가 결정되면 업데이트 필요 
		}
		string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		string result = "[Read] LBA " + to_string(address) + " : " + targetData;
		cout << result << endl;
		inputFile.close();
	}

	return true;
}

// full write
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

bool SSDShell::EraseSsdRange(int start_lba, int end_lba) {
	SSDDriver ssdDriver;
	ssdDriver.erase_range(start_lba, end_lba);
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
			cout << "ERASE RANGE" << command.GetStartLba()<< " "<< command.GetEndLba() << endl;
			EraseSsdRange(command.GetStartLba(), command.GetEndLba());
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

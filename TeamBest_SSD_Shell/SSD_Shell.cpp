#include "SSD_Shell.h"
#include "ShellLogger.h"

void SSDShell::Run(void) {
	int loopCount = 0;
	std::string line;
	while (loopCount < 5) {
		//0. test code for loop count
		cout << "loop count :" << loopCount++ << endl;

		// 1. print command cursor
		std::cout << "Shell>";

		// 2. input command
		// TODO : Enable later with Exit
		std::getline(std::cin, line);
		std::istringstream iss(line);

		// 3. parsing command
		ProcessParseInvalid(line);
		// 3-1. invalid check
		if (IsInvalidCommand() == true)
		{
			continue;
		}

		// 4. process command
		if (ExcuteCommand(parsingresult) == true) {
			break; // exit
		}
	}
}

bool SSDShell::IsInvalidCommand() { return parsingresult.invalidtype != NO_ERROR; }

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
		return ""; //  todo. 에러처리에 대한 리턴을 어떻게 정의할지가 결정되면 업데이트 필요 
	}
	string targetData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	inputFile.close();

	string result = "[Read] LBA " + to_string(address) + " : " + targetData;
	cout << result << endl;
	return result;
}

bool SSDShell::WriteSsd(string data)
{
	// TODO: ssd.exe 를 실행하는 부분을 구현해야함.
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

bool SSDShell::ExcuteCommand(ParsingResult command) {
	bool ret = false;

	switch (command.command) {
	case WRITE:
		WriteSsd(command.data);
		break;

	case READ:
		ReadSsdOutputFile(GetAddress());
		break;

	case FULL_WRITE:
		FullWrite(command.data);
		break;

	case FULL_READ:
		FullRead();
		break;

	case HELP:
		PrintHelp();
		break;

	case SCRIPT_EXECUTE:
		cout << "Script Execute : " << command.script_name << endl;
		break;

	case EXIT:
		cout << "Exit" << endl;
		ret = true;
		break;
	}
	return ret;
}

bool SSDShell::ProcessParseInvalid(std::string command) {
	ShellLogger::getInstance().log("SSDShell", "ProcessParseInvalid", command);

	std::vector<std::string> tokens;
	tokens = ParsingInputCommand(command);

	if (tokens.empty()) {
		UpdateInvalidType_and_PrintErrorMessage(NO_INPUT_COMMAND);
		return true;
	}

	std::string cmd = tokens[0];
	if (UpdateCommand(tokens[0]) == false) {
		UpdateInvalidType_and_PrintErrorMessage(INVALID_COMMAND);
		return true;
	}

	if (parsingresult.command == WRITE) {
		if (tokens.size() != 3) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		try {
			parsingresult.address = std::stoi(tokens[1]);
			parsingresult.data = tokens[2];

			// LBA Range Check
			if (!IsValidAddressRange()) {
				UpdateInvalidType_and_PrintErrorMessage(INVAILD_ADDRESS);
				return true;
			}

			// Value data range check
			if (!std::regex_match(parsingresult.data, std::regex("^0x[0-9A-Fa-f]{8}$"))) {
				UpdateInvalidType_and_PrintErrorMessage(INVALID_DATA);
				return true;
			}
		}
		catch (...) {
			return true;
		}
		return false;
	}
	else if (parsingresult.command == READ) {
		if (tokens.size() != 2) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		try {
			parsingresult.address = std::stoi(tokens[1]);

			if (!IsValidAddressRange()) {
				UpdateInvalidType_and_PrintErrorMessage(INVAILD_ADDRESS);
				return true;
			}

			parsingresult.command = READ;

		}
		catch (...) {
			return true;
		}
		return false;
	}
	else if (parsingresult.command == FULL_WRITE) {
		if (tokens.size() != 2) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		if (!std::regex_match(tokens[1], std::regex("^0x[0-9A-Fa-f]{8}$"))) {
			UpdateInvalidType_and_PrintErrorMessage(INVALID_DATA);
			return true;
		}

		parsingresult.command = FULL_WRITE;
		parsingresult.data = tokens[1];

		return false;
	}
	else if (parsingresult.command == FULL_READ || parsingresult.command == EXIT || parsingresult.command == HELP) {

		if (tokens.size() > 1) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		else {
			return false;
		}
	}

	return true;
}


vector<std::string>  SSDShell::ParsingInputCommand(std::string command) {
	std::istringstream iss(command);
	std::string token;
	std::vector<std::string> tokens;

	while (iss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

void SSDShell::UpdateInvalidType_and_PrintErrorMessage(int error_type) {
	switch (error_type) {
	case NO_INPUT_COMMAND:
		std::cout << "No Input command \n";
		parsingresult.invalidtype = NO_INPUT_COMMAND;
		break;
	case INVALID_COMMAND:
		std::cout << "Invalid Command \n";
		parsingresult.invalidtype = INVALID_COMMAND;
		break;
	case INVAILD_ADDRESS:
		std::cout << "Invalid LBA Range \n";
		parsingresult.invalidtype = INVAILD_ADDRESS;
		break;
	case INVALID_DATA:
		std::cout << "Invalid Data \n";
		parsingresult.invalidtype = INVALID_DATA;
		break;
	case NUMBER_OF_PARAMETERS_INCORRECT:
		std::cout << "The number of parameters are not correct  \n";
		parsingresult.invalidtype = NUMBER_OF_PARAMETERS_INCORRECT;
		break;
	default:
		break;
	}
}


bool SSDShell::UpdateCommand(std::string cmd) {

	// Change Upper characters to Lower characters
	std::transform(cmd.begin(), cmd.end(), cmd.begin(),
		[](unsigned char c) { return std::tolower(c); });

	// 정규표현식: 맨 앞에 "숫자 + 언더바" 형식인지 확인  (Check Number+Underbar)
	std::smatch match;
	std::regex pattern(R"(^(\d+)_([a-z]+))");

	if (std::regex_match(cmd, match, pattern)) {
		parsingresult.script_name = cmd;
		parsingresult.command = SCRIPT_EXECUTE;
	}
	else if (cmd == "write") { parsingresult.command = WRITE; }
	else if (cmd == "read") { parsingresult.command = READ; }
	else if (cmd == "fullwrite") { parsingresult.command = FULL_WRITE; }
	else if (cmd == "fullread") { parsingresult.command = FULL_READ; }
	else if (cmd == "exit") { parsingresult.command = EXIT; }
	else if (cmd == "help") { parsingresult.command = HELP; }
	else { return false; }
	
	return true;
}

bool SSDShell::IsValidAddressRange() {
	if (parsingresult.address < 0 || parsingresult.address >= 100)
		return false;
	else
		return true;
}
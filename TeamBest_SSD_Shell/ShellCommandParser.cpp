#include "ShellCommandParser.h"
#include <iostream>
#include <regex>
#include <iterator>
#include <sstream>
#include "ShellLogger.h"

ParsingResult parsingresult;

bool ShellCommandParser::ProcessParseInvalid(std::string command) {
	LOG_MESSAGE("SSDShell", command);

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

	if (parsingResult.GetCommand() == WRITE) {
		if (tokens.size() != 3) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		try {
			parsingResult.SetStartLba(std::stoi(tokens[1]));
			parsingResult.SetStartLba(std::stoi(tokens[1]));
			parsingResult.SetData(tokens[2]);

			if (IsInvalidAddressRange(parsingresult.GetStartLba())) {
				UpdateInvalidType_and_PrintErrorMessage(INVAILD_ADDRESS);
				return true;
			}

			if (!std::regex_match(parsingResult.GetData(), std::regex("^0x[0-9A-Fa-f]{8}$"))) {
				UpdateInvalidType_and_PrintErrorMessage(INVALID_DATA);
				return true;
			}
		}
		catch (...) {
			return true;
		}
		return false;
	}
	else if (parsingResult.GetCommand() == READ) {
		if (tokens.size() != 2) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		try {
			parsingResult.SetStartLba(std::stoi(tokens[1]));

			if (IsInvalidAddressRange(parsingresult.GetStartLba())) {
				UpdateInvalidType_and_PrintErrorMessage(INVAILD_ADDRESS);
				return true;
			}

			parsingResult.SetCommand(READ);
		}
		catch (...) {
			return true;
		}
		return false;
	}
	else if (parsingResult.GetCommand() == FULL_WRITE) {
		if (tokens.size() != 2) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		if (!std::regex_match(tokens[1], std::regex("^0x[0-9A-Fa-f]{8}$"))) {
			UpdateInvalidType_and_PrintErrorMessage(INVALID_DATA);
			return true;
		}

		parsingResult.SetCommand(FULL_WRITE);
		parsingResult.SetData(tokens[1]);

		return false;
	}
	else if (parsingResult.GetCommand() == FULL_READ || parsingResult.GetCommand() == EXIT || parsingResult.GetCommand() == HELP) {
		if (tokens.size() > 1) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		else {
			return false;
		}
	}
	else if (parsingResult.GetCommand() == ERASE || parsingResult.GetCommand() == ERASE_RANGE) {

		if (tokens.size() != 3) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}
		try {
			parsingresult.SetStartLba(std::stoi(tokens[1]));
			parsingresult.SetEndLbaOrSize(std::stoi(tokens[2]));

			if (parsingResult.GetCommand() == ERASE) {
				int lba = parsingresult.GetStartLba();
				int size = parsingresult.GetEndLba();

				// LBA1 Range Check (0~99)
				if (IsInvalidAddressRange(lba)) {
					UpdateInvalidType_and_PrintErrorMessage(INVAILD_ADDRESS);
					return true;
				}

				// TO Do : negative size, invalid size proessing
				//				if (size < 0) {
				//					lba = lba + size + 1;
				//					size = -size;
				//				}

				//				if ( (lba + size) >= 100 ) size = size - lba;
			}
			else {
				int start_lba = parsingresult.GetStartLba();
				int end_lba = parsingresult.GetEndLba();

				// LBA1 and LBA2 Range Check
				if (IsInvalidAddressRange(parsingresult.GetStartLba()) || IsInvalidAddressRange(parsingresult.GetEndLba())) {
					UpdateInvalidType_and_PrintErrorMessage(INVAILD_ADDRESS);
					return true;
				}
			}
		}
		catch (...) {
			return true;
		}
		return false;
	}

	else if (parsingResult.GetCommand() == SCRIPT_EXECUTE) {
		if (tokens.size() > 1) {
			UpdateInvalidType_and_PrintErrorMessage(NUMBER_OF_PARAMETERS_INCORRECT);
			return true;
		}

		parsingResult.SetScriptName(tokens[0]);
		return false;
	}

	return true;
}

std::vector<std::string> ShellCommandParser::ParsingInputCommand(std::string command) {
	std::string str;
	std::vector<std::string> command_tokens;

	command.erase(remove(command.begin(), command.end(), '\r'), command.end());
	command.erase(remove(command.begin(), command.end(), '\n'), command.end());

	std::istringstream iss(command);
	while (iss >> str) {
		command_tokens.push_back(str);
	}

	return command_tokens;
}

void ShellCommandParser::UpdateInvalidType_and_PrintErrorMessage(int error_type) {
	switch (error_type) {
	case NO_INPUT_COMMAND:
		std::cout << "No Input command \n";
		parsingResult.SetInvalidType(NO_INPUT_COMMAND);
		break;
	case INVALID_COMMAND:
		std::cout << "Invalid Command \n";
		parsingResult.SetInvalidType(INVALID_COMMAND);
		break;
	case INVAILD_ADDRESS:
		std::cout << "Invalid LBA Range \n";
		parsingResult.SetInvalidType(INVAILD_ADDRESS);
		break;
	case INVALID_DATA:
		std::cout << "Invalid Data \n";
		parsingResult.SetInvalidType(INVALID_DATA);
		break;
	case NUMBER_OF_PARAMETERS_INCORRECT:
		std::cout << "The number of parameters are not correct  \n";
		parsingResult.SetInvalidType(NUMBER_OF_PARAMETERS_INCORRECT);
		break;
	default:
		break;
	}
}

bool ShellCommandParser::UpdateCommand(std::string cmd) {
	std::transform(cmd.begin(), cmd.end(), cmd.begin(),
		[](unsigned char c) { return std::tolower(c); });

	std::smatch match;
	std::regex pattern(R"(^(\d+)_([a-z]*))");

	if (std::regex_match(cmd, match, pattern)) {
		parsingResult.SetScriptName(cmd);
		parsingResult.SetCommand(SCRIPT_EXECUTE);
	}
	else if (cmd == "write") { parsingResult.SetCommand(WRITE); }
	else if (cmd == "read") { parsingResult.SetCommand(READ); }
	else if (cmd == "fullwrite") { parsingResult.SetCommand(FULL_WRITE); }
	else if (cmd == "fullread") { parsingResult.SetCommand(FULL_READ); }
	else if (cmd == "exit") { parsingResult.SetCommand(EXIT); }
	else if (cmd == "help") { parsingResult.SetCommand(HELP); }
	else if (cmd == "erase") { parsingresult.SetCommand(ERASE); }
	else if (cmd == "erase_range") { parsingresult.SetCommand(ERASE_RANGE); }
	else if (cmd == "flush") { parsingresult.SetCommand(FLUSH); }
	else { return false; }


	parsingResult.SetInvalidType(NO_ERROR);
	return true;
}

bool ShellCommandParser::IsInvalidAddressRange(int lba) {
	if (lba < 0 || lba >= 100)
		return true;
	else
		return false;
}

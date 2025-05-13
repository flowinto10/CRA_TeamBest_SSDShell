#include "ShellCommandParser.h"
#include <iostream>
#include <regex>
#include <iterator>
#include <sstream>
#include "ShellLogger.h"

ParsingResult parsingresult;

bool ShellCommandParser::ProcessParseInvalid(std::string command) {
    LOG_MESSAGE("SSDShell", command);
    std::vector<std::string> tokens = ParsingInputCommand(command);
    if (tokens.empty()) {
        return Fail(NO_INPUT_COMMAND);
    }
    if (!UpdateCommand(tokens[0])) {
        return Fail(INVALID_COMMAND);
    }

    switch (parsingResult.GetCommand()) {
    case WRITE:
        return HandleWriteCommand(tokens);
    case READ:
        return HandleReadCommand(tokens);
    case FULL_WRITE:
        return HandleFullWriteCommand(tokens);
    case FULL_READ:
    case EXIT:
    case HELP:
    case FLUSH:
        return HandleSimpleCommand(tokens);
    case SCRIPT_EXECUTE:
        return HandleScriptCommand(tokens);
    case ERASE:
    case ERASE_RANGE:
        return HandleEraseCommand(tokens);
    default:
        return Fail(INVALID_COMMAND);
    }
}

bool ShellCommandParser::Fail(InvalidType type) {
    UpdateInvalidType_and_PrintErrorMessage(type);
    return true;
}

bool ShellCommandParser::HandleWriteCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    try {
        parsingResult.SetStartLba(std::stoi(tokens[1]));
        parsingResult.SetData(tokens[2]);
        if (parsingResult.IsInvalidAddressRange(parsingResult.GetStartLba())) {
            return Fail(INVAILD_ADDRESS);
        }
        if (!std::regex_match(parsingResult.GetData(), std::regex("^0x[0-9A-Fa-f]{8}$"))) {
            return Fail(INVALID_DATA);
        }
    }
    catch (...) {
        return true;
    }
    return false;
}

bool ShellCommandParser::HandleReadCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    try {
        parsingResult.SetStartLba(std::stoi(tokens[1]));
        if (parsingResult.IsInvalidAddressRange(parsingResult.GetStartLba())) {
            return Fail(INVAILD_ADDRESS);
        }
    }
    catch (...) {
        return true;
    }
    return false;
}

bool ShellCommandParser::HandleFullWriteCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    if (!std::regex_match(tokens[1], std::regex("^0x[0-9A-Fa-f]{8}$"))) {
        return Fail(INVALID_DATA);
    }
    parsingResult.SetData(tokens[1]);
    return false;
}

bool ShellCommandParser::HandleSimpleCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    return false;
}

bool ShellCommandParser::HandleScriptCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    parsingResult.SetScriptName(tokens[0]);
    return false;
}

bool ShellCommandParser::HandleEraseCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    try {
        parsingResult.SetStartLba(std::stoi(tokens[1]));
        parsingResult.SetEndLbaOrSize(std::stoi(tokens[2]));

        if (parsingResult.GetCommand() == ERASE) {
            if (parsingResult.IsInvalidAddressRange(parsingResult.GetStartLba())) {
                return Fail(INVAILD_ADDRESS);
            }
        }
        else {
            if (parsingResult.IsInvalidAddressRange(parsingResult.GetStartLba()) ||
                parsingResult.IsInvalidAddressRange(parsingresult.GetEndLba())) {
                return Fail(INVAILD_ADDRESS);
            }
        }
    }
    catch (...) {
        return true;
    }
    return false;
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

bool ShellCommandParser::IsValidAddressRange(int lba) {
    return !(lba < 0 || lba >= 100);
}

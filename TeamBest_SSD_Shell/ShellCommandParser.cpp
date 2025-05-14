#include "ShellCommandParser.h"
#include <iostream>
#include <regex>
#include <iterator>
#include <sstream>
#include "ShellLogger.h"

bool ShellCommandParser::ProcessParseInvalid(const std::string& command) {
    LOG_MESSAGE(command);
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
//  UpdateInvalidType_and_PrintErrorMessage funtion stores log message so it is commented.
//	LOG_MESSAGE("InvalidType : " + std::to_string(type));
    UpdateInvalidType_and_PrintErrorMessage(type);
    return true;
}


bool ShellCommandParser::IsValidIntegerString(const std::string& str) {
    static const std::regex intRegex("^-?\\d+$");
    return std::regex_match(str, intRegex);
}


bool ShellCommandParser::HandleWriteCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    try {
        if (!IsValidIntegerString(tokens[1])) return Fail(INVALID_DATA);
        
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
        return Fail(INVALID_DATA);
    }
    return false;
}

bool ShellCommandParser::HandleReadCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }
    try {
        if (!IsValidIntegerString(tokens[1])) return Fail(INVALID_DATA);

        parsingResult.SetStartLba(std::stoi(tokens[1]));
        if (parsingResult.IsInvalidAddressRange(parsingResult.GetStartLba())) {
            return Fail(INVAILD_ADDRESS);
        }
    }
    catch (...) {
        return Fail(INVALID_DATA);
    }
    return false;
}

bool ShellCommandParser::HandleFullWriteCommand(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        return Fail(NUMBER_OF_PARAMETERS_INCORRECT);
    }

    if (!IsValidIntegerString(tokens[1])) return Fail(INVALID_DATA);

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
        if (!IsValidIntegerString(tokens[1])) return Fail(INVALID_DATA);
        if (!IsValidIntegerString(tokens[2])) return Fail(INVALID_DATA);

        int lba = std::stoi(tokens[1]);
        int value = std::stoi(tokens[2]);
        parsingResult.SetStartLba(lba);
        parsingResult.SetEndLbaOrSize(value);

        if (parsingResult.GetCommand() == ERASE) {
            // negative size handling
            if (value < 0) {
                lba = lba + value + 1;
                value = -value;
            }
            // range check and adjust size
            if (parsingResult.IsInvalidAddressRange(lba) ) {
                return Fail(INVAILD_ADDRESS);
            }
            if (lba + value > 100) { // if range is over, adjust the size
                value = 100 - lba;   
            }
            parsingResult.SetStartLba(lba);
            parsingResult.SetEndLbaOrSize(value);
        }
        else {
            int start_lba = lba;
            int end_lba = value;
            // range check
            if (parsingResult.IsInvalidAddressRange(start_lba) || parsingResult.IsInvalidAddressRange(end_lba)) {
                return Fail(INVAILD_ADDRESS);
            }
            // swap if out of order
            if (start_lba > end_lba) {
                std::swap(start_lba, end_lba);
            }
        //    /* Commented processing is considered in the SSD driver code.  
        //    int count = end_lba - start_lba + 1;
        //    if (start_lba + count > 100) {
        //        count = 100 - start_lba;
        //    }

            parsingResult.SetStartLba(start_lba);
            parsingResult.SetEndLbaOrSize(end_lba);
        }
    }
    catch (...) {
        return Fail(INVALID_DATA);
    }
    return false;
}

std::vector<std::string> ShellCommandParser::ParsingInputCommand(const std::string& command) {
    std::vector<std::string> tokens;
    std::string s = command;
    s.erase(remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());

    std::istringstream iss(s);
    for (std::string tok; iss >> tok;) {
        tokens.push_back(tok);
    }

    return tokens;
}

void ShellCommandParser::CoutMessage_and_LogMessage(const std::string& message) {
    std::cout << message << std::endl;
    LOG_MESSAGE(message);
}

void ShellCommandParser::UpdateInvalidType_and_PrintErrorMessage(InvalidType error_type) {
	
    parsingResult.SetInvalidType(error_type);
    
    switch (error_type) {
	case NO_INPUT_COMMAND:
        CoutMessage_and_LogMessage("Invalid Command: No Input command");
		break;
	case INVALID_COMMAND:
        CoutMessage_and_LogMessage("Invalid Command: Command is not defined");
		break;
	case INVAILD_ADDRESS:
        CoutMessage_and_LogMessage("Invalid Command: Invalid LBA Range (Vaild range : 0~99)");
		break;
	case INVALID_DATA:
        CoutMessage_and_LogMessage("Invalid Command: Invalid Data");
		break;
	case NUMBER_OF_PARAMETERS_INCORRECT:
        CoutMessage_and_LogMessage("Invalid Command: The number of parameters are not correct");
		break;
	default:
		break;
	}
}

bool ShellCommandParser::UpdateCommand(const std::string& cmdIn) {
    std::string cmd = cmdIn;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) { return std::tolower(c); });

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
	else if (cmd == "erase") { parsingResult.SetCommand(ERASE); }
	else if (cmd == "erase_range") { parsingResult.SetCommand(ERASE_RANGE); }
	else if (cmd == "flush") { parsingResult.SetCommand(FLUSH); }
	else { return false; }

    parsingResult.SetInvalidType(NO_ERROR_TYPE);
    return true;
}


bool ShellCommandParser::IsValidAddressRange(int lba) const {
    return !(lba < 0 || lba >= 100);
}
#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>

enum Command {
    WRITE = 1,
    READ = 2,
    EXIT = 3,
    HELP = 4,
    FULL_WRITE = 5,
    FULL_READ = 6,
    SCRIPT_EXECUTE = 7,
    ERASE = 8,
    ERASE_RANGE = 9,
    FLUSH = 10
};

enum InvalidType {
    NO_ERROR = 0,
    INVALID_COMMAND = 1,
    INVALID_DATA = 2,
    INVAILD_ADDRESS = 3,
    NO_INPUT_COMMAND = 4,
    NUMBER_OF_PARAMETERS_INCORRECT = 5
};

class ParsingResult {
public:
    ParsingResult(int cmd = 0, int start = 0, int end = 0,
        const std::string& d = "", const std::string& name = "", InvalidType type = NO_ERROR)
        : command(cmd), startlba(start), endlba_or_size(end),
        data(d), script_name(name), invalidtype(type) {}
    ParsingResult(const ParsingResult& other) = default;
    ~ParsingResult() = default;

    int GetCommand() const { return command; }
    int GetStartLba() const { return startlba; }
    int GetEndLba() const { return endlba_or_size; }
    int GetSize() const { return endlba_or_size; }
    const std::string& GetData() const { return data; }
    const std::string& GetScriptName() const { return script_name; }
    InvalidType GetInvalidType() const { return invalidtype; }

    void SetCommand(int cmd) { command = cmd; }
    void SetStartLba(int addr) { startlba = addr; }
    void SetEndLbaOrSize(int size) { endlba_or_size = size; }
    void SetData(const std::string& d) { data = d; }
    void SetScriptName(const std::string& name) { script_name = name; }
    void SetInvalidType(InvalidType type) { invalidtype = type; }

    bool IsInvalidCommand() const { return invalidtype != NO_ERROR; }
    bool IsInvalidAddressRange(int lba) const { return (lba < 0 || lba >= 100); }

private:
    int command;
    int startlba;
    int endlba_or_size;
    std::string data;
    std::string script_name;
    InvalidType invalidtype;
};

class ShellCommandParser {
public:
    ShellCommandParser() = default;
    ~ShellCommandParser() = default;

    bool ProcessParseInvalid(const std::string& command);
    const ParsingResult& GetParsingResult() const { return parsingResult; }

private:
    std::vector<std::string> ParsingInputCommand(const std::string& command);
    void UpdateInvalidType_and_PrintErrorMessage(InvalidType type);
    bool UpdateCommand(const std::string& cmd);
    bool IsValidAddressRange(int lba) const;
    bool Fail(InvalidType type);

    bool HandleWriteCommand(const std::vector<std::string>& tokens);
    bool HandleReadCommand(const std::vector<std::string>& tokens);
    bool HandleFullWriteCommand(const std::vector<std::string>& tokens);
    bool HandleSimpleCommand(const std::vector<std::string>& tokens);
    bool HandleScriptCommand(const std::vector<std::string>& tokens);
    bool HandleEraseCommand(const std::vector<std::string>& tokens);

    ParsingResult parsingResult;
};

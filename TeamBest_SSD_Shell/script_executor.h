#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "script_command.h"
#include "testscript_all.hl"

class ScriptExcutor {
public:
    ScriptExcutor() {
        registerCommand("1_", std::unique_ptr<ScriptCommand>(new FullWRiteAndReadCompare()));
        registerCommand("2_", std::unique_ptr<ScriptCommand>(new PartialLBAWrite()));
        registerCommand("3_", std::unique_ptr<ScriptCommand>(new WriteReadAging()));
    }

    void registerCommand(const std::string& prefix, std::unique_ptr<ScriptCommand> command) {
        commandMap[prefix] = std::move(command);
    }

    bool execute(const std::string& userInput) {
        for (auto it = commandMap.begin(); it != commandMap.end(); ++it) {
            const std::string& prefix = it->first;
            ScriptCommand* command = it->second.get();

            if (userInput.find(prefix) == 0) {
                command->execute();
                return true;
            }
        }

        std::cout << "Unknown command: " << userInput << std::endl;
        return false;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<ScriptCommand>> commandMap;
};
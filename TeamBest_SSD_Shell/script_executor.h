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
				command->RunScript();
				return true;
			}
		}

		std::cout << "Unknown command: " << userInput << std::endl;
		return false;
	}

	bool ExecuteAll(const std::string& filename) { 
		// shell_script.txt 를 읽어서 그 안에 있는 tc를 순서대로 실행함

		std::string inputCommand = "";
		std::ifstream file(filename);
		bool ret = false;
		if (file.is_open()) {
			// 루프를 돌면서 한줄씩 읽은걸 ScriptExcutor 를 통해 실행한다.
			while (std::getline(file, inputCommand)) {
				// BOM 제거
				if (inputCommand.compare(0, 3, "\xEF\xBB\xBF") == 0)
					inputCommand = inputCommand.substr(3);
				// CR 제거 (Windows에서 생기는 문제)
				inputCommand.erase(std::remove(inputCommand.begin(), inputCommand.end(), '\r'), inputCommand.end());
				// 공백 제거
				inputCommand.erase(std::remove_if(inputCommand.begin(), inputCommand.end(), ::isspace), inputCommand.end());

				cout << inputCommand << "___ Run... ";
				ret = execute(inputCommand);

				if (ret == false) { 
					cout << "FAIL!!\n";
					return false; // tc 실패시 즉시 종료
				} 
				cout << "Pass\n";
			}
		}
		else {
			std::cerr << "Unable to open file" << std::endl;
			return false;
		}
	}

private:
	std::unordered_map<std::string, std::unique_ptr<ScriptCommand>> commandMap;
};
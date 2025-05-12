#include "ISSD_Shell.h"
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <iostream>


// #define WRITE 1
// #define READ 2
// #define EXIT 3
// #define HELP 4
// #define FULL_WRITE 5
// #define FULL_READ 6

enum Command {
    WRITE = 1,
    READ = 2,
    EXIT = 3,
    HELP = 4,
    FULL_WRITE = 5,
    FULL_READ = 6
};



class SSDShell : public IShell{
public:
    void PrintHelp() override {}
    std::string ReadInputFile(int address) override { return 0x0; }
    bool WriteOutputFile(std::string output, int address) override { return true; }
    void ProcessInputCommand(std::string command) override {}
    bool ProcessParseInvalid(std::string command) override { 

        std::istringstream iss(command);
        std::string token;
        std::vector<std::string> tokens;

        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) return true;

        std::string cmd = tokens[0];

        if (cmd == "write") {
            if (tokens.size() != 3) return true;
            try {
                int lba = std::stoi(tokens[1]);
                std::string value = tokens[2];

                // LBA 범위 검사
                if (lba < 0 || lba >= 100) {
                    std::cout<<"Invalid LBA \n";
                    return true;
                }

                // Value 포맷 검사: 0x + 8자리 hex
                if (!std::regex_match(value, std::regex("^0x[0-9A-Fa-f]{8}$"))) {
                    std::cout << "Invalid Value \n";
                    return true;
                }

                parsingresult.command = WRITE;
                parsingresult.address = lba;
                parsingresult.data = value;

            }
            catch (...) {
                return true;
            }
            return false;
        }
        else if (cmd == "read") {
            if (tokens.size() != 2) return true;
            try {
                int lba = std::stoi(tokens[1]);
                if (lba < 0 || lba >= 100) return true;

                parsingresult.command = READ;
                parsingresult.address = lba;
            }
            catch (...) {
                return true;
            }
            return false;
        }
        else if (cmd == "fullwrite") {
            if (tokens.size() != 2) return true;
            if (!std::regex_match(tokens[1], std::regex("^0x[0-9A-Fa-f]{8}$"))) return true;

            parsingresult.command = FULL_WRITE;
            parsingresult.data = tokens[1];

            return false;
        }
        else if (cmd == "fullread" || cmd == "exit" || cmd == "help") {
            if (cmd == "fullread")
                parsingresult.command = FULL_READ;
            else if (cmd == "exit")
                parsingresult.command = EXIT;
            else if (cmd == "help")
                parsingresult.command = HELP;

            return tokens.size() != 1;
        }

        return true; 
    }

    int GetCommand(void) { return parsingresult.command;}
    int GetAddress(void) { return parsingresult.address; }
    std::string GetData(void) { return parsingresult.data; }

private:
    struct ParsingResult {
        int command;
        int address;
        std::string data;
    } parsingresult;
};

  


#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream
#include <vector>
#include <regex>

#include "ssdDriver.h"
#include "ShellLogger.h"

void SSDDriver::read(int address) {
	// 1. 명령어 문자열 구성: "ssd.exe r <address>"
	std::ostringstream commandStream;
	commandStream << "ssd.exe r " << address;
	std::string command = commandStream.str();

	// 2. 명령 실행
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
		LOG_MESSAGE("ssd.exe 실행 실패. 종료 코드: " + to_string(result));
	}
}

void SSDDriver::write(int address, std::string data) {
	// 1. 명령어 문자열 구성: "ssd.exe w <address> <data>"
	std::ostringstream commandStream;
	commandStream << "ssd.exe w " << address << " " << data;
	std::string command = commandStream.str();

	// 2. 명령 실행
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
		LOG_MESSAGE("ssd.exe 실행 실패. 종료 코드: " + to_string(result));
	}
}


void SSDDriver::erase(int lba, int size) {
	const int MAX_SIZE = 10;
	
	while (size > 0) {
		int currentSize = std::min(size, MAX_SIZE);

		std::ostringstream commandStream;
		commandStream << "ssd.exe e " << lba << " " << currentSize;
		std::string command = commandStream.str();

		// 2. 명령 실행
		int result = system(command.c_str());
		if (result != 0) {
			std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
			LOG_MESSAGE("ssd.exe 실행 실패. 종료 코드: " + to_string(result));
		}

		lba += currentSize;
		size -= currentSize;
	}
}

void SSDDriver::flush(void) {
	std::ostringstream commandStream;
	commandStream << "ssd.exe F";
	std::string command = commandStream.str();

	// 2. 명령 실행
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
		LOG_MESSAGE("ssd.exe 실행 실패. 종료 코드: " + to_string(result));
	}
}
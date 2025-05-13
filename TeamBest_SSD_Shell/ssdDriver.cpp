#include "ssdDriver.h"
void SSDDriver::read(int address) {
	// 1. 명령어 문자열 구성: "ssd.exe r <address>"
	std::ostringstream commandStream;
	commandStream << "ssd.exe r " << address;
	std::string command = commandStream.str();

	// 2. 명령 실행
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
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
	}
}


void SSDDriver::erase(int lba, int size) {
	std::ostringstream commandStream;
	commandStream << "ssd.exe e " << lba << " " << size;
	std::string command = commandStream.str();

	// 2. 명령 실행
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
	}
}

void SSDDriver::erase_range(int start_lba, int end_lba) {
	int size = end_lba - start_lba + 1;
	std::ostringstream commandStream;
	commandStream << "ssd.exe e " << start_lba << " " << size;
	std::string command = commandStream.str();

	// 2. 명령 실행
	int result = system(command.c_str());
	if (result != 0) {
		std::cerr << "ssd.exe 실행 실패. 종료 코드: " << result << std::endl;
	}
}
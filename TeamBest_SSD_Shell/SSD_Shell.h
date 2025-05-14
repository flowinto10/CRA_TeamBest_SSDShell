#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream

#include <vector>
#include <regex>

#include "ShellCommandParser.h"
#include "ssdDriver.h"

using namespace std;

class SSDShell {
public:
	// 1. Singleton 인스턴스 반환 메소드
	static SSDShell& getInstance() {
		static SSDShell instance;  // 프로그램 실행 동안 딱 한 번만 생성
		return instance;
	}

	void Run(void);
	bool ExcuteCommand(ParsingResult command);

	std::string ReadSsdOutputFile(int address);
	bool WriteSsd(int lba, string data);
	bool EraseSsd(int lba, int size);
	bool EraseSsdRange(int start_lba, int end_lba);
	bool Flush(void);

	bool FullRead();
	bool FullWrite(string data);

	void PrintHelp();

//	bool IsInvalidCommand();


private:
	// 3. private 생성자: 외부에서 객체를 만들 수 없도록 함
	SSDShell() {
		// 생성자에서 필요한 초기화
	}

	// 4. 복사 생성자와 대입 연산자를 삭제하여 복사 방지
	SSDShell(const SSDShell&) = delete;
	SSDShell& operator=(const SSDShell&) = delete;

	const string SSD_NAND = "ssd_nand.txt"; // SSD NAND 파일 이름
	const string SSD_OUTPUT = "ssd_output.txt"; // SSD 출력 파일 이름
	const int MIN_LBA = 0;
	const int MAX_LBA = 99;

	ShellCommandParser commandParser;
};
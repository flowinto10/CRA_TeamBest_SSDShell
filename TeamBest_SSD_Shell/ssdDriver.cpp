#include "ssdDriver.h"

void SSDDriver::read(int address) {
	NiceMock<MockSSD> mock;
	mock.read(address);
	//--------------------------------------------------------------------------------------------------
	// todo : 나중에 실제로 구현된 ssd.exe 를 가지고 구현할 때는 이렇게?? 해야할 것 같아서 일단 남김.
	//int result = system("ssd.exe");

	//ostringstream commandStream;
	//commandStream << "ssd.exe " << address;
	//string command = commandStream.str();

	//// 2. 명령 실행
	//int result = system(command.c_str());
	//if (result != 0) {
	//	cerr << "ssd.exe 실행 실패. 종료 코드: " << result << endl;
	//	return;
	//}
}
void SSDDriver::write(int address, string data) {
	NiceMock<MockSSD> mock;
	mock.write(address, data);
	//--------------------------------------------------------------------------------------------------
	// todo : 나중에 실제로 구현된 ssd.exe 를 가지고 구현할 때는 이렇게?? 해야할 것 같아서 일단 남김.
	//int result = system("ssd.exe");
	//ostringstream commandStream;
	//commandStream << "ssd.exe " << address << " " << data;
	//string command = commandStream.str();
	//// 2. 명령 실행
	//int result = system(command.c_str());
	//if (result != 0) {
	//	cerr << "ssd.exe 실행 실패. 종료 코드: " << result << endl;
	//	return;
	//}
}


void SSDDriver::erase(int lba, int size) {
	NiceMock<MockSSD> mock;
	mock.erase(lba, size);
	// 
}

void SSDDriver::erase_range(int start_lba, int end_lba) {
	NiceMock<MockSSD> mock;
	mock.erase_range(start_lba, end_lba);
	// 
}
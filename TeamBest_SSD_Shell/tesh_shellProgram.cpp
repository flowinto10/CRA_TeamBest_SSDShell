#include "gmock/gmock.h"
#include <iostream>
#include "mock_ssd.cpp"

using namespace std;
using namespace testing;

TEST(ProcessCommand, ReadTest_01) {
	MockSSD mock;
	int lba = 0;
	EXPECT_CALL(mock, read(lba)).Times(1);
	mock.read(lba);
	EXPECT_TRUE(true); // todo : ssd_output.txt 열어서 저 주소에 00000000 이 있는지 확인하는 코드 추가 필요함
}

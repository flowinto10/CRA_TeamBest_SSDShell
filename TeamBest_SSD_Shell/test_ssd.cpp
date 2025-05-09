#include "gmock/gmock.h"
#include <iostream>
#include "mock_ssd.cpp"

using namespace std;
using namespace testing;

TEST(MockSSDCommand, ReadTest_01) {
	MockSSD mock;
	int lba = 0;
	EXPECT_CALL(mock, read(lba)).Times(1);
	mock.read(lba);
	EXPECT_TRUE(true); // todo : ssd_output.txt ��� �� �ּҿ� 00000000 �� �ִ��� Ȯ���ϴ� �ڵ� �߰� �ʿ���
}

TEST(MockSSDCommand, WriteTest_01) {
	MockSSD mock;
	string data = "0x11112222";
	int lba = 0;
	EXPECT_CALL(mock, write(lba, data)).Times(1);
	mock.write(lba, data);
	EXPECT_TRUE(true); // todo : ssd_nand.txt ��� �� �ּҿ� 0x11112222 �� �ִ��� Ȯ���ϴ� �ڵ� �߰� �ʿ���
}
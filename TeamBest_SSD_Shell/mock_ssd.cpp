#include "gmock/gmock.h" // Google Mock ���
#include "gtest/gtest.h" // Google Test ��� (�ʿ��� �� ����)
#include <string>
using namespace std;
using namespace testing;

class MockSSD {
public:
	MOCK_METHOD(void, write, (int lba, string data), ());
	MOCK_METHOD(void, read, (int lba), ());
};

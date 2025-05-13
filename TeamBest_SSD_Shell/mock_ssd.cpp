#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <string>
using namespace std;
using namespace testing;

class MockSSD {
public:
	MOCK_METHOD(void, write, (int lba, string data), ());
	MOCK_METHOD(void, read, (int lba), ());
	MOCK_METHOD(void, erase, (int lba, int size), ());
	MOCK_METHOD(void, erase_range, (int start_lba, int end_lba), ());
};

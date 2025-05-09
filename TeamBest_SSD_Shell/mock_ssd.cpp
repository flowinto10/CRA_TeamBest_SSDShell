#include "gmock/gmock.h" // Google Mock 헤더
#include "gtest/gtest.h" // Google Test 헤더 (필요할 수 있음)
#include <string>
using namespace std;
using namespace testing;

class MockSSD {
public:
	MOCK_METHOD(void, write, (int lba, string data), ());
	MOCK_METHOD(void, read, (int lba), ());
};

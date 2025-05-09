#include "gmock/gmock.h"
#include <string>
using namespace std;
using namespace testing;

class MockSSD {
public:
	MOCK_METHOD(void, write, (int lba, string data), ());
	MOCK_METHOD(void, read, (int lba), ());
};

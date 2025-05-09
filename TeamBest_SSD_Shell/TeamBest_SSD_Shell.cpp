#include <iostream>
#include "gmock/gmock.h"

using namespace testing;


class Shell {
public:
    virtual void PrintHelp() = 0;
    virtual bool ReadInputFile(std::string inout, int address) = 0;  // File Read
    virtual bool WriteOutputFile(std::string output, int address) = 0;  // File Write
    virtual void ProcessInputCommand(std::string inout) = 0;  // Command Input Processing
    virtual bool ProcessParseInvalid(std::string inout) = 0;  // Command Parsing & invalid 처리

private:
    const int MAX_LBA = 100;
    virtual bool IsValidLba(int lba) = 0;  // LBA 유효성 검사
    virtual bool IsValidValue(const std::string& value) = 0;  // VALUE 유효성 검사: 0x + 8자리 HEX
};


TEST(ShellTS, TC0) {
    EXPECT_EQ(1, 1);
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#include <iostream>
#include "gmock/gmock.h"

using namespace testing;


class Shell {
public:
    virtual void Print_Help() = 0;
    virtual bool Read_Input_File(std::string inout, int address) = 0;  // File Read
    virtual bool Write_Output_File(std::string output, int address) = 0;  // File Write
    virtual void Process_Input_Command(std::string inout) = 0;  // Command Input Processing
    virtual bool Process_Parse_Invalid(std::string inout) = 0;  // Command Parsing & invalid 처리

private:
    const int MAX_LBA = 100;
    virtual bool Is_valid_lba(int lba) = 0;  // LBA 유효성 검사
    virtual bool is_valid_value(const std::string& value) = 0;  // VALUE 유효성 검사: 0x + 8자리 HEX
};


TEST(ShellTS, TC0) {
    EXPECT_EQ(1, 1);
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#include <iostream>
#include "gmock/gmock.h"

using namespace testing;


class Shell {
public:
    virtual void Print_Help() = 0;
    virtual void Interprete_Input_Command(std::string inout) = 0;
    virtual void Run_SSD_Command(const std::string& cmd) = 0;  // ssd 프로그램 실행
    virtual std::string Get_SSD_Output() = 0; // read 후 결과 가져오기

    virtual bool Is_valid_lba(int lba) = 0;  // LBA 유효성 검사
    virtual bool is_valid_value(const std::string& value) = 0;  // VALUE 유효성 검사: 0x + 8자리 HEX


private:
    const int MAX_LBA = 100;
};


TEST(ShellTS, TC0) {
    EXPECT_EQ(1, 1);
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

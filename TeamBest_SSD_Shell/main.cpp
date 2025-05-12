#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.cpp"

using namespace testing;

TEST(ShellTS, CraeteShellInstance) {
    EXPECT_NO_THROW(SSDShell* ssdShell = new SSDShell());
}


class ParingInvalidFixture : public Test {
public:
    SSDShell test;
    
    void WriteCheck(int exp_command, int exp_address, std::string exp_data, std::string input) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetAddress(), exp_address);
            EXPECT_EQ(test.GetData(), exp_data);
        }
        else {
            EXPECT_EQ(1, 1);    // When the input string is valid, TC will be excuted.
        }
    }

    void ReadCheck(int exp_command, int exp_address, std::string input) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetAddress(), exp_address);
        }
        else {
            EXPECT_EQ(1, 1);
        }
    }

    void FullWriteCheck (int exp_command, std::string exp_data, std::string input) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetData(), exp_data);
        }
        else {
            EXPECT_EQ(1, 1);
        }
    }

    void FullRead_Exit_Help_Check(int exp_command, std::string input) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
        }
        else {
            EXPECT_EQ(1, 1);
        }
    }
};


TEST_F(ParingInvalidFixture, valid_write) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "write 3 0xAAAABBBB");
}

TEST_F(ParingInvalidFixture, invalid_write) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "writ 3 0xAAAABBBB");
}

TEST_F(ParingInvalidFixture, read) {
    ReadCheck(READ, 1, "read 1");
}

TEST_F(ParingInvalidFixture, fullwrite) {
    FullWriteCheck(FULL_WRITE, "0xCCCCDDDD", "fullwrite 0xCCCCDDDD");
}

TEST_F(ParingInvalidFixture, fullread) {
    FullRead_Exit_Help_Check(FULL_READ, "fullread");
}

TEST_F(ParingInvalidFixture, exit1) {
    FullRead_Exit_Help_Check(EXIT, "exit");
}

TEST_F(ParingInvalidFixture, help) {
    FullRead_Exit_Help_Check(HELP, "help");
}



int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

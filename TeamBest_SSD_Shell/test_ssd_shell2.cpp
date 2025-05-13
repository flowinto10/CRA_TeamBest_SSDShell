#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.h"


using namespace testing;


class ParingInvalidFixture : public Test {
public:
    SSDShell test;

    void WriteCheck(int exp_command, int exp_address, std::string exp_data, std::string input, int invalid_type) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetAddress(), exp_address);
            EXPECT_EQ(test.GetData(), exp_data);
        }
        else {

            EXPECT_EQ(test.GetInvalidType(), invalid_type);    // When the input string is valid, TC will be excuted.
        }
    }

    void ReadCheck(int exp_command, int exp_address, std::string input, int invalid_type) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetAddress(), exp_address);
        }
        else {
            EXPECT_EQ(test.GetInvalidType(), invalid_type);
        }
    }

    void FullWriteCheck(int exp_command, std::string exp_data, std::string input, int invalid_type) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetData(), exp_data);
        }
        else {
            EXPECT_EQ(test.GetInvalidType(), invalid_type);
        }
    }

    void FullRead_Exit_Help_Check(int exp_command, std::string input, int invalid_type) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
        }
        else {
            EXPECT_EQ(test.GetInvalidType(), invalid_type);
        }
    }


    void EraseCheck(int exp_command, int lba, int size, std::string input, int invalid_type) {
        if (test.ProcessParseInvalid(input) == false) {
            EXPECT_EQ(test.GetCommand(), exp_command);
            EXPECT_EQ(test.GetAddress(), lba);
            EXPECT_EQ(test.GetSize(), size);
        }
        else {
            EXPECT_EQ(test.GetInvalidType(), invalid_type);
        }
    }
};


TEST_F(ParingInvalidFixture, valid_erase1) {
    EraseCheck(ERASE, 3, 4, "erase 3 4", NO_ERROR);
}

TEST_F(ParingInvalidFixture, valid_erase2) {
    EraseCheck(ERASE, 99, 1, "erase 99 4", NO_ERROR);
}



TEST_F(ParingInvalidFixture, valid_write) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "write 3 0xAAAABBBB", NO_ERROR);
}

TEST_F(ParingInvalidFixture, invalid_write) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "writ 3 0xAAAABBBB", INVALID_COMMAND);
}

TEST_F(ParingInvalidFixture, read) {
    ReadCheck(READ, 1, "read 1", NO_ERROR);
}

TEST_F(ParingInvalidFixture, fullwrite) {
    FullWriteCheck(FULL_WRITE, "0xCCCCDDDD", "fullwrite 0xCCCCDDDD", NO_ERROR);
}

TEST_F(ParingInvalidFixture, fullread) {
    FullRead_Exit_Help_Check(FULL_READ, "fullread", NO_ERROR);
}

TEST_F(ParingInvalidFixture, exit1) {
    FullRead_Exit_Help_Check(EXIT, "exit", NO_ERROR);
}

TEST_F(ParingInvalidFixture, help) {
    FullRead_Exit_Help_Check(HELP, "help", NO_ERROR);
}

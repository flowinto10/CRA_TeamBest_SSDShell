#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.cpp"

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

            EXPECT_EQ(test.GetInvalidType(), invalid_type);  
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
};


TEST_F(ParingInvalidFixture, valid_write) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "write 3 0xAAAABBBB",NO_ERROR);
}

TEST_F(ParingInvalidFixture, write_invalid_command) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "writ 3 0xAAAABBBB", INVALID_COMMAND);
}

TEST_F(ParingInvalidFixture, write_numberof_parameters_incorrect) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "write 3 0xAAAABBBB eeee", NUMBER_OF_PARAMETERS_INCORRECT);
}

TEST_F(ParingInvalidFixture, write_address_range_incorrect) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "write 101 0xAAAABBBB", INVAILD_ADDRESS);
}

TEST_F(ParingInvalidFixture, data_range_incorrect) {
    WriteCheck(WRITE, 3, "0xAAAABBBB", "write 99 0xAAAAB***", INVALID_DATA);
}

TEST_F(ParingInvalidFixture, valid_read) {
    ReadCheck(READ, 1, "read 1", NO_ERROR);
}

TEST_F(ParingInvalidFixture, read_invalid_command) { 
    ReadCheck(READ, 1, "rea 1", INVALID_COMMAND);
}

TEST_F(ParingInvalidFixture, read_numberof_parameters_incorrect) {
    ReadCheck(READ, 1, "read 1 3", NUMBER_OF_PARAMETERS_INCORRECT);
}

TEST_F(ParingInvalidFixture, read_address_range_incorrect) {
    ReadCheck(READ, 1, "read -1", INVAILD_ADDRESS);
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


int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

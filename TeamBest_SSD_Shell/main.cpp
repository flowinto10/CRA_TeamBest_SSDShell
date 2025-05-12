#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.cpp"

using namespace testing;

TEST(ShellTS, CraeteShellInstance) {
    EXPECT_NO_THROW(SSDShell* ssdShell = new SSDShell());
}



TEST(ProcessParseInvalid, write) {
    SSDShell test;
    test.ProcessParseInvalid("write 3 0xAAAABBBB");

    int command1;
    int address1;
    std::string data1;

    command1 = test.GetCommand();
    address1 = test.GetAddress();
    data1 = test.GetData();

    EXPECT_EQ(command1, WRITE);
    EXPECT_EQ(address1, 3);
    EXPECT_EQ(data1, "0xAAAABBBB");
}

TEST(ProcessParseInvalid, read) {
    SSDShell test;
    test.ProcessParseInvalid("read 2");

    int command1;
    int address1;

    command1 = test.GetCommand();
    address1 = test.GetAddress();

    EXPECT_EQ(command1, READ);
    EXPECT_EQ(address1, 2);
}

TEST(ProcessParseInvalid, fullwrite) {
    SSDShell test;
    test.ProcessParseInvalid("fullwrite 0xCCCCDDDD");

    int command1;
    std::string data1;

    command1 = test.GetCommand();
    data1 = test.GetData();

    EXPECT_EQ(command1, FULL_WRITE);
    EXPECT_EQ(data1, "0xCCCCDDDD");
}

TEST(ProcessParseInvalid, fullread) {
    SSDShell test;
    test.ProcessParseInvalid("fullread");

    int command1;

    command1 = test.GetCommand();
    EXPECT_EQ(command1, FULL_READ);
}


TEST(ProcessParseInvalid, exit) {
    SSDShell test;
    test.ProcessParseInvalid("exit");

    int command1;

    command1 = test.GetCommand();
    EXPECT_EQ(command1, EXIT);
}


TEST(ProcessParseInvalid, help) {
    SSDShell test;
    test.ProcessParseInvalid("help");

    int command1;

    command1 = test.GetCommand();
    EXPECT_EQ(command1, HELP);
}


int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

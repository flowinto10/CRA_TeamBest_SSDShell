#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.cpp"

using namespace testing;

TEST(ShellTS, CraeteShellInstance) {
    EXPECT_NO_THROW(SSDShell* ssdShell = new SSDShell());
}

TEST(ShellTS, CheckRunMethod) {
    SSDShell* ssdShell = new SSDShell();
    EXPECT_NO_THROW(ssdShell->Run());
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

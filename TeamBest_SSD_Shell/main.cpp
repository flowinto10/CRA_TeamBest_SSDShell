#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.cpp"

using namespace testing;

TEST(ShellTS, CraeteShellInstance) {
    EXPECT_NO_THROW(SSDShell* ssdShell = new SSDShell());
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

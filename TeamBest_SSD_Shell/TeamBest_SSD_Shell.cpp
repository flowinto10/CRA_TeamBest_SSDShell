#include <iostream>
#include "gmock/gmock.h"

using namespace testing;

TEST(ShellTS, TC0) {
    EXPECT_EQ(1, 1);
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

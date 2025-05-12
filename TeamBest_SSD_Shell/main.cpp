#include <iostream>

#ifdef _DEBUG
#include "gmock/gmock.h"
#endif

#include "SSD_Shell.h"

using namespace testing;

int main()
{
#ifdef _DEBUG
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
#else
    SSDShell shell;
    shell.Run();
#endif
}

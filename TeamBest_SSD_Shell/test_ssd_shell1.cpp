#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.h"

using namespace testing;

TEST(ShellTS, CraeteShellInstance) {
    SSDShell& shell = SSDShell::getInstance();
    EXPECT_NO_THROW(&shell);
}

TEST(ShellTS, ExitCommandTest) {
    SSDShell& ssdShell = SSDShell::getInstance();
    ParsingResult parsingresult{ Command::EXIT, 0, 0, " ", " ", InvalidType::NO_ERROR_TYPE };

	EXPECT_EQ(true,  ssdShell.ExcuteCommand(parsingresult));
}


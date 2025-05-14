#include <iostream>
#include "gmock/gmock.h"
#include "SSD_Shell.h"

using namespace testing;


TEST(ShellTS, CraeteShellInstance) {
    SSDShell& shell = SSDShell::getInstance();
    EXPECT_NO_THROW(&shell);
//    EXPECT_NO_THROW(&SSDShell * ssdShell = new SSDShell());
}


#if 0
TEST(ShellTS, CheckRunMethod) {

//    SSDShell* ssdShell = new SSDShell();
    SSDShell& ssdShell = SSDShell::getInstance();
    EXPECT_NO_THROW(ssdShell.Run());
}
#endif



TEST(ShellTS, ExitCommandTest) {
//	SSDShell* ssdShell = new SSDShell();
    SSDShell& ssdShell = SSDShell::getInstance();
    ParsingResult parsingresult{ Command::EXIT, 0, 0, " ", " ", InvalidType::NO_ERROR_TYPE };

	EXPECT_EQ(true,  ssdShell.ExcuteCommand(parsingresult));
}


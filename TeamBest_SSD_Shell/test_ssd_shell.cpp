#include "gmock/gmock.h"
#include <iostream>
#include "ISSD_Shell.h"
#include "SSD_Shell.cpp"

using namespace std;
using namespace testing;

TEST(ShellTest, PrintTC01) {
	SSDShell* shell = new SSDShell();
	shell->PrintHelp();
	EXPECT_TRUE(true); // 출력 되는것만 본다
}

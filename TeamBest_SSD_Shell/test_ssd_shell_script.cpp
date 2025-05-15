#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "SSD_Shell.h"
#include "script_command.h"
#include "script_executor.h"
#include "testscript_all.hl"


TEST(ShellScript, 1_FullWriteAndReadCompare) {
	std::string inputCommand = "1_";

	ScriptExcutor executor;
	bool ret = executor.execute(inputCommand);
	EXPECT_TRUE(ret);
}
TEST(ShellScript, 1_FullWriteAndReadCompare_2) {
	std::string inputCommand = "1_FullWriteAndReadCompare";

	ScriptExcutor executor;
	bool ret = executor.execute(inputCommand);
	EXPECT_TRUE(ret);
}

TEST(ShellScript, 2_PartialLBAWrite) {
	std::string inputCommand = "2_";

	ScriptExcutor executor;
	bool ret = executor.execute(inputCommand);
	EXPECT_TRUE(ret);
}

TEST(ShellScript, 3_WriteReadAging) {
	std::string inputCommand = "3_";

	ScriptExcutor executor;
	bool ret = executor.execute(inputCommand);
	EXPECT_TRUE(ret);
}

TEST(ShellScript, 4_EraseAndWriteAging) {	
	std::string inputCommand = "4_";
	ScriptExcutor executor;
	bool ret = executor.execute(inputCommand);
	EXPECT_TRUE(ret);
}


#include <iostream>

#ifdef _DEBUG
#include "gmock/gmock.h"
#endif

#include "SSD_Shell.h"
#include "script_executor.h"

using namespace testing;

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	if (argc == 1) {
		SSDShell shell;
		shell.Run();
	}
	else if (argc == 2) {
		ScriptExcutor* scriptExcutor = new ScriptExcutor();;
		scriptExcutor->ExecuteAll(argv[1]);
	}
	else {
		std::cerr << "Invalid Usage: Argc should be 1 or 2, Example: shell.exe, shell.exe [Filename]" << std::endl;
		LOG_MESSAGE("Invalid Usage: Argc should be 1 or 2, Example: shell.exe, shell.exe [Filename]");
		return 1;
	}

	return 0;
#endif
}

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
		std::cerr << "사용법: shell.exe [파일명]" << std::endl;
		LOG_MESSAGE("사용법: shell.exe [파일명]");
		return 1;
	}

	return 0;
#endif
}

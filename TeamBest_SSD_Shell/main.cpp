#undef byte
#include <Windows.h>

#include <iostream>

#ifdef _DEBUG
#include "gmock/gmock.h"
using namespace testing;
#endif

#include "SSD_Shell.h"
#include "script_executor.h"



#include <filesystem>
#include <vector>
#include "Parent.h"
#include "Registry.h"


namespace fs = std::filesystem;

using RegisterFuncType = void(*)();

void LoadPlugin(const std::string& path) {
	
	HMODULE hModule = LoadLibraryA(path.c_str());
	if (!hModule) {
		std::cerr << "Failed to load DLL: " << path
			<< " Error code: " << GetLastError() << std::endl;
		return;
	}

	LOG_MESSAGE("Loaded DLL : " + path);

	// 함수 포인터 타입
	using RegisterFuncType = void(*)();

	// 함수 가져오기
	auto registerFunc = (RegisterFuncType)GetProcAddress(hModule, "RegisterPlugins");
	if (registerFunc) {		
		registerFunc();
	}
	else {
		DWORD errorCode = GetLastError();
		std::cerr << "RegisterPlugins not found in: " << path
			<< " (GetLastError = " << errorCode << ")" << std::endl;
	}
}


void ExecuteScriptAll(string filename) {
	std::string pluginDir = std::filesystem::current_path().string();
	for (const auto& entry : fs::directory_iterator(pluginDir)) {
		if (entry.path().extension() == ".dll") {			
			LoadPlugin(entry.path().string());
		}
	}
	const auto& reg = Registry::Instance().GetRegistry();
	for (const auto& [name, creator] : reg) {
		std::unique_ptr<Parent> obj(creator());
		LOG_MESSAGE("DLL script tc class : " + name);
		obj->Hello();
	}
}


int main(int argc, char* argv[])
{
#ifdef _DEBUG
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	if (argc == 1) {
		// SSDShell의 유일한 인스턴스를 얻어 사용  (single pattern)
		SSDShell& shell = SSDShell::getInstance();

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

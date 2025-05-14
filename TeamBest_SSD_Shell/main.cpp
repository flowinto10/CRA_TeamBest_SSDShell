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
	std::cout << "Trying to load: " << path << std::endl;
	HMODULE hModule = LoadLibraryA(path.c_str());
	if (!hModule) {
		std::cerr << "Failed to load DLL: " << path
			<< " Error code: " << GetLastError() << std::endl;
		return;
	}

	std::cout << "Loaded DLL: " << path << std::endl;

	// 함수 포인터 타입
	using RegisterFuncType = void(*)();

	// 함수 가져오기
	auto registerFunc = (RegisterFuncType)GetProcAddress(hModule, "RegisterPlugins");
	if (registerFunc) {
		std::cout << "RegisterPlugins found in: " << path << std::endl;
		registerFunc();
	}
	else {
		DWORD errorCode = GetLastError();
		std::cerr << "RegisterPlugins not found in: " << path
			<< " (GetLastError = " << errorCode << ")" << std::endl;
	}
}


bool ExecuteScriptAll(string filename) {

	std::cout << "loading plugins..." << std::endl;
	//std::string pluginDir = "./plugins";
	std::string pluginDir = std::filesystem::current_path().string();
	std::cout << "Plugin Directory: " << pluginDir << std::endl;

	for (const auto& entry : fs::directory_iterator(pluginDir)) {
		if (entry.path().extension() == ".dll") {
			std::cout << "Loading plugin: " << entry.path().string() << std::endl;
			LoadPlugin(entry.path().string());
		}
	}
	std::cout << "start..." << std::endl;
	const auto& reg = Registry::Instance().GetRegistry();
	for (const auto& [name, creator] : reg) {
		std::unique_ptr<Parent> obj(creator());
		std::cout << "Calling Hello on: " << name << " => ";
		obj->Hello();
	}
	std::cout << "end..." << std::endl;
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

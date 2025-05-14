#include "Registry.h"
#include "AAA.h"
#include "BBB.h"
#include "CCC.h"

extern "C" __declspec(dllexport) void RegisterPlugins() {
    std::cout << __FILE__ << " : RegisterPlugins called" << std::endl;
    Registry::Instance().Register("AAA", []() -> Parent* { return new AAA(); });
    Registry::Instance().Register("BBB", []() -> Parent* { return new BBB(); });
    Registry::Instance().Register("CCC", []() -> Parent* { return new CCC(); });
}
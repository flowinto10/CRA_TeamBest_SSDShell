#pragma once
#pragma warning(push)
#pragma warning(disable: 4251)
#ifdef SCRIPTIMPLEMENTDLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <map>
#include <string>
#include <functional>
#include "Parent.h"

class DLL_API Registry {
public:
    using CreatorFunc = std::function<Parent* ()>;

    static Registry& Instance();

    void Register(const std::string& name, CreatorFunc func);
    const std::map<std::string, CreatorFunc>& GetRegistry() const;

private:
    Registry() = default;
    std::map<std::string, CreatorFunc> registry_;
};

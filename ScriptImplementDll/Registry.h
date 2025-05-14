//#pragma once
//#include <map>
//#include <string>
//#include <iostream>
//
//#include "Parent.h"
//
//class Registry {
//public:
//    static Registry& Instance() {
//        std::cout << "(lib)***Registry Instance" << std::endl;
//        static Registry instance;
//        return instance;
//    }
//
//    void Register(const std::string& name, CreateFunc func) {
//        std::cout << "(lib)***Registering: " << name << std::endl;
//        registry[name] = func;
//    }
//
//    const std::map<std::string, CreateFunc>& GetRegistry() const {
//        std::cout << "(lib)***Getting Registry" << registry.size() << std::endl;
//        return registry;
//    }
//
//private:
//    std::map<std::string, CreateFunc> registry;
//};

#pragma once

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

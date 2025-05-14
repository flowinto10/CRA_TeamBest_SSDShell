#pragma once
#include "Registry.h"
#include <iostream>

// 클래스 등록 매크로
#define REGISTER_CLASS(classname)                                \
    extern "C" __declspec(dllexport) Parent* Create_##classname() { \
        std::cout << "~~~~ Creating instance of " #classname << std::endl; \
        return new classname();                                  \
    }                                                             \
    class classname##Registrator {                                \
    public:                                                       \
        classname##Registrator() {                                \
            std::cout << "~~~~ Registering " #classname << std::endl; \
            Registry::Instance().Register(#classname, Create_##classname); \
        }                                                         \
    };                                                            \
    static classname##Registrator global_##classname##Registrator;
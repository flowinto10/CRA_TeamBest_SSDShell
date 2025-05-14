#pragma once
#include "Parent.h"

class CCC : public Parent {
public:
    void Hello() override {
        std::cout << "[[[[[[[[[[[[[[[[[[[CCC" << std::endl;
    }
};

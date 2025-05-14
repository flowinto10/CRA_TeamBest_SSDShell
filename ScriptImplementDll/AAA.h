#pragma once
#include "Parent.h"

class AAA : public Parent {
public:
    void Hello() override {
        std::cout << "[[[[[[[[[[[[[[[[[[AAA" << std::endl;
    }
};
#pragma once
#include "Parent.h"

class BBB : public Parent {
public:
    void Hello() override {
        std::cout << "[[[[[[[[[[[[[[[[[[[BBB" << std::endl;
    }
};

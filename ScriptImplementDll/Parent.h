#pragma once
#include <iostream>
#include <string>

class Parent {
public:
    virtual ~Parent() = default;
    virtual void Hello() = 0;
};

// 함수 포인터로 클래스 객체를 생성하기 위한 타입 정의
using CreateFunc = Parent * (*)();

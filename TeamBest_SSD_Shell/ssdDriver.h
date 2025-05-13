#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>  // system 함수
#include <sstream>  // ostringstream

#include <vector>
#include <regex>

#include "mock_ssd.cpp"

using namespace std;
class SSDDriver {
public:
	void read(int address);
	void write(int address, string data);
};

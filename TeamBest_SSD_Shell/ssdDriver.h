#pragma once
#include <string>

using namespace std;
class SSDDriver {
public:
	void read(int address);
	void write(int address, string data);
	void erase(int lba, int size);
	void erase_range(int start_lba, int end_lba);
	void flush(void);
};

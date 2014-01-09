#pragma once

#include <string>

using namespace std;

class SizeManager
{
public:
	SizeManager() {}
	~SizeManager() {}

	static int SizeOfType(string type)
	{
		return sizeof(int);
	}
};


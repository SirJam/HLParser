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
		if (type == "char" || type == "int" || type == "bool")
		{
			return sizeof(int);
		}
		else
		{
			return 4;
		}
	}
};


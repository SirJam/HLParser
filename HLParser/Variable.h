#pragma once

#include "Type.h"
#include <string>

using namespace std;

struct Variable
{
	string name;
	TYPE type;
	int count;
};

int inline sizeOf(TYPE const type)
{
	if (type == TYPE_INT) {
		return sizeof(int);
	}
	else if (type == TYPE_CHAR) {
		return sizeof(int);
	}
	else if (type == TYPE_BOOL) {
		return sizeof(int); // Because we store boolean variables as int number '1' or '0'
	}
	return 0;
}

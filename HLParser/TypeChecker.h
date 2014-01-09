#pragma once

#include <string>

using namespace std;

class TypeChecker
{
public:
	TypeChecker() {}
	~TypeChecker() {}

	static bool IsAllowedType(string type)
	{
		return (type == "char" || type == "int" || type == "bool");
	}
};
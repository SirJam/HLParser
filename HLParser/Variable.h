#pragma once

#include <string>

using namespace std;

class Variable
{
public:
	Variable() {}
	~Variable() {}

	string m_name;
	string m_type;
	string m_value;
	int m_size;
	int m_xDimension;
};


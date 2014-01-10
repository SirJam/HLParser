#pragma once

#include <string>
#include <vector>
#include "Variable.h"
#include "Token.h"

using namespace std;

class VariablesTable
{
public:
	VariablesTable();
	~VariablesTable();

	bool TryToRegisterVariable(vector<Token> stack);
	void CheckExistingOfVariable(vector<Token> stack, bool isNotElementOfArray);
	int getOffset(string varName);

	vector<Variable> *m_variablesTable;
};


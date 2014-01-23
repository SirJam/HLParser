#pragma once

#include <string>
#include <vector>
#include "Variable.h"
#include "Token.h"
#include <algorithm>

using namespace std;

class VariablesTable
{
private:
	Token *GetLastVariable(Token *token);
	void GetBodyWithoutTail(Token & token);
	Token *m_tempToken;
public:
	VariablesTable();
	~VariablesTable();

	bool TryToRegisterVariable(vector<Token> stack);
	void CheckExistingOfVariable(vector<Token> stack, bool isNotElementOfArray);
	bool DoesVariableExists(string variablesName);
	static Token *GetRootToken(Token *token);

	int getOffset(string varName);
	string getType(string varName);	
	int getXDimention(string varName);
	vector<string> GetExpressionStack(vector<Token> stack);

	vector<Variable> *m_variablesTable;
};


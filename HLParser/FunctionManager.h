#pragma once

#include "Token.h"
#include "VariableManager.h"
#include <map>
#include <vector>
#include <string.h>

using namespace std;

class FunctionManager
{
public:
	FunctionManager(vector<Token> &tokens, VariableManager &variableManager);

	//void checkMainFunction();
	//void generateFunctionCall();
	//TYPE getFunctionType(string const& functionName);
private:
	int findNumberOfArguments();
	//TYPE findArgumentType(int const argumentNumber, int const totalArgumentNumber);
	//Token *getVariableToken(Token const& functionToken);

	//bool hasFormingTokenWithName(Token *token, string name);

	map<string, int> numberOfArguments;
	map<string, TYPE> functionTypes;

	vector<Token> *parserTokens;
	VariableManager *variableManager;
};

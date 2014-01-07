#pragma once

#include "Token.h"
#include "Variable.h"
#include "Type.h"
#include <string>

class FunctionManager;
class VariableManager;

using namespace std;

class TypeChecker
{
public:
	TypeChecker(vector<Token> &tokens);

	void setFunctionManager(FunctionManager &functionManager);
	void setVariableManager(VariableManager &variableManager);

	void checkFunctionType(Token const& tokenType, TYPE expectedType);
	void checkVariableType(Variable const& variable, bool isInArray, Token const& idToken);
	void compareAssignmentTypes(TYPE const variableType, TYPE const expressionType, int const lineNumber);
	void compareTwoTypes(TYPE const type1, TYPE const type2, int const lineNumber);

	void checkNegationType();

	Token *getTypeToken(Token &token);

	TYPE checkConstant(Token const& tokenConstant);
	Token *getTokenId(Token &token);

	string lastString;
	int lastInt;
	bool lastBool;
	bool isInArray;

private:
	vector<Token> *parserTokens;

	FunctionManager *functionManager;
	VariableManager *variableManager;

	void resetLastValues();
};

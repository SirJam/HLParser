#pragma once

#include "Variable.h"
#include "Token.h"

class FunctionManager;

class VariableManager
{
public:
	VariableManager(vector<Token> &tokens);
	/*void setFunctionManager(FunctionManager &functionManager);

	void generateVariableInExpression();
	void generateAssignment();
	void generateVariableSpace();
	void generateRemoveVariableSpace();
	void generateStringSpace();

	void addVariable();

	int findVariablePosition(Token const *variableToken);
	Variable *findVariable(Token const *variableToken);
	int calculateVariableOffset(int const variableNumber);

	int findStringPosition(Token const *variableToken);*/
private:
	vector<Variable> variables;
	vector<int> variableCount; // to keep the number of declared variables at block starts
	int lastVariableCount;

	int maxStringCount; // maximum number of declared strings during program execution
	int currentStringCount;

	vector<Token> *parserTokens;
	FunctionManager *functionManager;

	//void addVariableCommentary(Variable const& variable, Token const& idToken);
};

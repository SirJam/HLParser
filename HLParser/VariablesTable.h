#pragma once

#include <string>
#include <vector>
#include "Variable.h"
#include "Token.h"
#include <algorithm>
#include "Sender.h"


using namespace std;

class VariablesTable
{
private:
	Token *GetLastVariable(Token *token);
	void GetBodyWithoutTail(Token & token);
	void CompareTypes(vector<Token> stack, vector<string> rightPart);
	bool IsAssignableArrayExpression(vector<string> stack, Token *arrayToken);
	bool IsAssignableIntBoolExpression(vector<string> stack, Token *idToken);

	Token *m_tempToken;
public:
	VariablesTable(Sender *sender);
	~VariablesTable();

	bool TryToRegisterVariable(vector<Token> stack);
	void CheckExistingOfVariable(vector<Token> stack, bool isNotElementOfArray);
	bool DoesVariableExists(string variablesName);
	static Token *GetRootToken(Token *token);

	int getOffset(string varName);
	int getSize(string varName);
	string getType(string varName);	
	int getXDimention(string varName);	
	vector<string> GetExpressionStack(vector<Token> stack, bool isAssignment);

	vector<Variable> *m_variablesTable;
	Sender *sender;
};


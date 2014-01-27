#pragma once

#include "Symbol.h"
#include "Production.h"
#include "Action.h"
#include "Token.h"
#include <stack> 
#include "Generator.h"
#include "RuleName.h"
#include "VariablesTable.h"

#include <vector>
#include <string.h>
#include <iostream>
#include "Sender.h"

using namespace std;

class Parser
{
public:
	Parser(Generator *generator, Sender *sender);	

	void GetNextToken(Token const& token);
	Token CreateTokenEOF();

	vector<Symbol> *m_symbolsTable;
	vector<Production> *m_productionsTable;
	vector<Action> *m_actionsTable;

	int m_lastAction;
private:
	vector<int> m_states;
	vector<string> m_createdLabels;
	vector<Token> m_tokens;
	vector<string> m_createdIfExpressionsLabels;
	stack<string> m_whileLabels;
	string m_exprType;

	VariablesTable *m_variablesTable;

	void ProcessProduction(Production *production);
	void ChangeExprType(string newType);
	void Shift(Action action, Token token);
	bool Reduce(Action action);
	void ComputeProduction(Production *production);
	
	void ParseExpression(vector<string> stack);

	void Accept();
	void HandleError(Token token);

	Symbol SymbolWithIndex(int index);
	Generator *generator;
	Sender *sender;
};

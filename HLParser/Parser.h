#pragma once

#include "Symbol.h"
#include "Production.h"
#include "Action.h"
#include "Token.h"
#include "VariablesTable.h"

#include <vector>
#include <string.h>
#include <iostream>

using namespace std;

class Parser
{
public:
	Parser();

	void getNextToken(Token const& token);
	Token createTokenEOF();

	vector<Symbol> *m_symbolsTable;
	vector<Production> *m_productionsTable;
	vector<Action> *m_actionsTable;

	int lastAction;
private:

	vector<int> m_states;
	vector<string> m_createdLabels;
	vector<Token> m_tokens;

	VariablesTable *m_variablesTable;

	void processProduction(Production *production);
	void Shift(Action action, Token token);
	bool Reduce(Action action);
	void computeProduction(Production *production);

	void Accept();
	void HandleError(Token token);

	Symbol symbolWithIndex(int index);
};

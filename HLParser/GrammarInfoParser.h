#pragma once

#include "Scanner.h"
#include "Symbol.h"
#include "Production.h"
#include "Action.h"
#include <vector>
#include <string>

using namespace std;

class GrammarInfoParser
{
public:
	GrammarInfoParser(const char *fileName);

	vector<Symbol> *readSymbolTable();
	vector<Production> *readProductionTable();
	vector<Action> *readActionTable();
private:
	Scanner scanner;
	int symbolCount;
	int productionCount;
	int actionCount;

	vector<Symbol> symbolTable;
	vector<Production> productionTable;
	vector<Action> actionTable;

	void readSymbolCount();

	void readProductionCount();

	void readActionCount();

	void skipUntilAssign();
	void skipUntilProperty(string const& propertyString);
	void skipLine();

	string readDigit();
	string readName();
};

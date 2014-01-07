#include "StdAfx.h"
#include "GrammarInfoParser.h"
#include <iostream>

GrammarInfoParser::GrammarInfoParser(const char *fileName)
:scanner(fileName), symbolCount(0) {}

void GrammarInfoParser::readSymbolCount()
{
	skipUntilProperty("Symbol Table");
	skipUntilAssign();
	string count = readDigit();
	symbolCount = atoi(count.c_str());
}

vector<Symbol> *GrammarInfoParser::readSymbolTable()
{
	readSymbolCount();
	vector<Symbol> *table = new vector<Symbol>;
	for (int i = 0; i < symbolCount; i++) {
		skipUntilAssign();
		string index = readDigit();
		skipUntilAssign();
		string name = readName();
		skipUntilAssign();
		skipUntilAssign();
		string type = readDigit();
		skipUntilAssign();

		Symbol symbol;
		symbol.index = atoi(index.c_str());
		symbol.name = name;
		symbol.type = atoi(type.c_str());
		table->push_back(symbol);
		//cout << symbol.index << " " << symbol.name << " " << symbol.type << endl;
	}

	return table;
}

void GrammarInfoParser::readProductionCount()
{
	skipUntilProperty("Production Table");
	skipUntilAssign();
	string count = readDigit();
	productionCount = atoi(count.c_str());
}

vector<Production> *GrammarInfoParser::readProductionTable()
{
	readProductionCount();
	vector<Production> *table = new vector<Production>;
	for (int i = 0; i < productionCount; i++) {
		skipUntilAssign();
		string index = readDigit();
		skipUntilAssign();
		skipUntilAssign();
		string nonTerminalIndex = readDigit();
		skipUntilAssign();
		skipLine(); // beacause in this 'description' line we may encounter many various '=' operators
		skipUntilAssign();
		int symbolCount = atoi(readDigit().c_str());
		skipUntilAssign();
		vector<int> handles;
		for (int j = 0; j < symbolCount; j++) {
			int handle = atoi(readDigit().c_str());
			handles.push_back(handle);
			scanner.readCharacter();
		}

		Production production;
		production.index = atoi(index.c_str());
		production.nonTerminalIndex = atoi(nonTerminalIndex.c_str());
		production.handles = handles;
		table->push_back(production);
		//cout << production.index << " " << production.nonTerminalIndex << endl;
	}

	return table;
}

void GrammarInfoParser::readActionCount()
{
	skipUntilProperty("LALR Table");
	skipUntilAssign();
	string count = readDigit();
	actionCount = atoi(count.c_str());
}

vector<Action> *GrammarInfoParser::readActionTable()
{
	readActionCount();
	vector<Action> *table = new vector<Action>;
	for (int i = 0; i < actionCount; i++) {
		skipUntilProperty("LALR State");
		skipUntilAssign();
		skipUntilAssign();
		int actions = atoi(readDigit().c_str());
		for (int j = 0; j < actions; j++) {
			skipUntilProperty("LALR Action");
			skipUntilAssign();
			skipUntilAssign();
			int symbolIndex = atoi(readDigit().c_str());
			skipUntilAssign();
			skipUntilAssign();
			int actionNumber = atoi(readDigit().c_str());
			skipUntilAssign();
			int target = atoi(readDigit().c_str());

			Action action;
			action.stateIndex = i;
			action.symbolIndex = symbolIndex;
			action.type = actionNumber;
			action.target = target;
			table->push_back(action);
			//cout << action.stateIndex << " " << action.symbolIndex << " " << action.type << " " << action.target << endl;
		}
	}

	return table;
}

void GrammarInfoParser::skipUntilProperty(string const& propertyString)
{
	bool readingBetweenBrackets = false;

	char ch = scanner.readCharacter();
	string word = "";

	while (ch != EOF) {
		if (ch == '[') {
			readingBetweenBrackets = true;
			ch = scanner.readCharacter();
			word = "";
		} else if (ch == ']') {
			readingBetweenBrackets = false;
			if (word == propertyString) {
				break;
			}
		}

		if (readingBetweenBrackets) {
			word += ch;
		}

		ch = scanner.readCharacter();
	}
}

void GrammarInfoParser::skipUntilAssign()
{
	char ch = scanner.readCharacter();
	while (ch != '=') {
		ch = scanner.readCharacter();
	}
	ch = scanner.readCharacter();
	while (ch == ' ') {
		ch = scanner.readCharacter();
	}
	scanner.returnCharacter();
}

void GrammarInfoParser::skipLine()
{
	char ch = scanner.readCharacter();
	while (ch != '\n') {
		ch = scanner.readCharacter();
	}
}

string GrammarInfoParser::readDigit()
{
	string digit = "";
	char ch = scanner.readCharacter();
	while (isdigit(ch)) {
		digit += ch;
		ch = scanner.readCharacter();
	}
	scanner.returnCharacter();

	return digit;
}

string GrammarInfoParser::readName()
{
	string name = "";
	char ch = scanner.readCharacter();
	while (ch != ' ' && ch != '\n') {
		name += ch;
		ch = scanner.readCharacter();
	}
	scanner.returnCharacter();

	return name;
}
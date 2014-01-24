#pragma once

#include "Symbol.h"
#include <vector>

using namespace std;

class TokenRecognizer
{
public:
	TokenRecognizer(vector<Symbol> const& symbolTable);
	Symbol TokenTypeByTokensValue(string const& lexeme);
private:
	vector<Symbol> symbols;
	Symbol TokenTypeByTokenName(string const& name);
	bool isNumber(string const& lexeme);
	bool isId(string const& lexeme);
};

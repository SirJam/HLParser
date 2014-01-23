#pragma once

#include "Symbol.h"
#include <vector>

using namespace std;

class TokenRecognizer
{
public:
	TokenRecognizer(vector<Symbol> const& symbolTable);
	Symbol TokenTypeByTokensValue(string const& value);
private:
	vector<Symbol> symbols;
	Symbol TokenTypeByTokenName(string const& name);
	bool isNumber(string const& value);
	bool isId(string const& value);
};

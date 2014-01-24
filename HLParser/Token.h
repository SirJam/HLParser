#pragma once

#include "Symbol.h"
#include <string.h>
#include <vector>

using namespace std;

class Token
{
public:
	Token(Symbol tokenType, string const& lexeme, int const line);

	int line;
	string lexeme;
	Symbol symbol;
	vector<Token> nodes;
};

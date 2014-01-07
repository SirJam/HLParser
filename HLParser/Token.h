#pragma once

#include "Symbol.h"
#include <string.h>
#include <vector>

using namespace std;

class Token
{
public:
	Token(Symbol tokenType, int const lineNumber);

	int lineNumber;
	string value;
	Symbol symbol;
	vector<Token> formingTokens;
};

#pragma once

#include "Token.h"


class ExpressionManager
{
public:
	ExpressionManager(vector<Token> &tokens);

	bool needGenerateRelExpressionCode();

private:
	vector<Token> *parserTokens;
};

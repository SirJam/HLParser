#include "StdAfx.h"
#include "ExpressionManager.h"
#include <string>
#include <vector>

using namespace std;

ExpressionManager::ExpressionManager(vector<Token> &tokens)
{
	this->parserTokens = &tokens;
}

bool ExpressionManager::needGenerateRelExpressionCode()
{
	string firstToken = parserTokens->end()[-1].symbol.name;
	string secondToken = parserTokens->end()[-2].symbol.name;
	string thirdToken = parserTokens->end()[-3].symbol.name;

	if (firstToken == "add-expression" &&
		thirdToken == "add-expression" &&
		secondToken == "relop") {
			return true;
	}
	return false;
}
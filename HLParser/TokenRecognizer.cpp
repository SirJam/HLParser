#include "StdAfx.h"
#include "TokenRecognizer.h"
#include "RuleName.h"

TokenRecognizer::TokenRecognizer(vector<Symbol> const& symbolTable)
{
	symbols = symbolTable;
}

Symbol TokenRecognizer::TokenTypeByTokensValue(string const& value)
{
	for (Symbol symbol : symbols) {
		if (value == symbol.name && symbol.type == 1 && value != RuleName::EOF_RULE() && value != RuleName::ERROR()
			&& value != RuleName::WHITESPACE() && value != RuleName::IDENTIFIER() && value != RuleName::DIGITS()) 
		{
			return symbol;
		}
	}

	if (isNumber(value)) return TokenTypeByTokenName(RuleName::DIGITS());
	if (isId(value)) return TokenTypeByTokenName(RuleName::IDENTIFIER());

	return TokenTypeByTokenName(RuleName::ERROR());
}

Symbol TokenRecognizer::TokenTypeByTokenName(string const& name)
{
	for (Symbol symbol : symbols) {
		if (symbol.name == name) return symbol;
	}
}

bool TokenRecognizer::isNumber(string const& value)
{
	for (char ch : value) {
		if (!isdigit(ch)) {
			return false;
		}
	}
	return true;
}

bool TokenRecognizer::isId(string const& value)
{
	bool isPotentialId = true;
	for (char ch : value) {
		if (!isdigit(ch) && !isalpha(ch)) {
			isPotentialId = false;
		}
	}

	return (isPotentialId && !isdigit(value[0]));
}
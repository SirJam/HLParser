#include "StdAfx.h"
#include "TokenRecognizer.h"
#include "RuleName.h"

TokenRecognizer::TokenRecognizer(vector<Symbol> const& symbolTable)
{
	symbols = symbolTable;
}

Symbol TokenRecognizer::TokenTypeByTokensValue(string const& lexeme)
{
	for (Symbol symbol : symbols) 
	{
		if (lexeme == symbol.m_term && symbol.m_type == 1 && lexeme != RuleName::EOF_RULE() && lexeme != RuleName::ERROR()
			&& lexeme != RuleName::WHITESPACE() && lexeme != RuleName::IDENTIFIER() && lexeme != RuleName::DIGITS()) 
		{
			return symbol;
		}
	}

	if (isNumber(lexeme)) return TokenTypeByTokenName(RuleName::DIGITS());
	if (isId(lexeme)) return TokenTypeByTokenName(RuleName::IDENTIFIER());

	return TokenTypeByTokenName(RuleName::ERROR());
}

Symbol TokenRecognizer::TokenTypeByTokenName(string const& name)
{
	for (Symbol symbol : symbols) 
	{
		if (symbol.m_term == name) return symbol;
	}
}

bool TokenRecognizer::isNumber(string const& lexeme)
{
	for (char ch : lexeme) 
	{
		if (!isdigit(ch)) 
		{
			return false;
		}
	}
	return true;
}

bool TokenRecognizer::isId(string const& lexeme)
{
	bool isPotentialId = true;
	for (char ch : lexeme) 
	{
		if (!isdigit(ch) && !isalpha(ch)) 
		{
			isPotentialId = false;
		}
	}

	return (isPotentialId && !isdigit(lexeme[0]));
}
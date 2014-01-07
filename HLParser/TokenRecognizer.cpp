#include "StdAfx.h"
#include "TokenRecognizer.h"

TokenRecognizer::TokenRecognizer(vector<Symbol> const& symbolTable)
{
	symbols = symbolTable;
}

Symbol TokenRecognizer::tokenTypeByValue(string const& value)
{
	for (Symbol symbol : symbols) {
		if (value == symbol.name
			&& symbol.type == 1
			&& value != "EOF" 
			&& value != "Error" 
			&& value != "Whitespace" 
			&& value != "identifier"
			&& value != "digits") {
			return symbol;
		}
	}

	if (isNumber(value)) return tokenTypeWithName("digits");
	if (isId(value)) return tokenTypeWithName("identifier");

	return tokenTypeWithName("Error");
}

Symbol TokenRecognizer::tokenTypeWithName(string const& name)
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
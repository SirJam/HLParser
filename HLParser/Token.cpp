#include "StdAfx.h"
#include "Token.h"

Token::Token(Symbol tokenType, string const& value, int const lineNumber)
:symbol(tokenType), 
value(value),
lineNumber(lineNumber) 
{
}
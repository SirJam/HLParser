#include "StdAfx.h"
#include "Token.h"

Token::Token(Symbol tokenType, int const lineNumber)
:symbol(tokenType), 
value(tokenType.name),
lineNumber(lineNumber) 
{
}
#include "StdAfx.h"
#include "Token.h"

Token::Token(Symbol tokenType, string const& lexeme, int const line)
:symbol(tokenType), 
lexeme(lexeme),
line(line) 
{
}
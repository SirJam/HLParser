#pragma once

#include "Scanner.h"
#include "Token.h"
#include "TokenRecognizer.h"
#include "Symbol.h"

class Lexer
{
public:
	Lexer(const char *fileName, vector<Symbol> const& symbolTable);

	Token nextToken();
	bool nextTokenExists();

private:
	string readNextWord();

	bool isEndOfFile(char ch);
	bool isEmptyCharacter(char ch);
	bool isDigitOrChar(char ch);
	bool isSpecialCharacter(char ch);

	Scanner scanner;
	TokenRecognizer tokenRecognizer;
};

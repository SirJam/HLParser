#pragma once

#include "Token.h"
#include "TokenRecognizer.h"
#include "Symbol.h"
#include <iostream>
#include <fstream>

class Lexer
{
public:
	Lexer(const char *fileName, vector<Symbol> const& symbolTable);

	Token nextToken();
	bool nextTokenExists();

private:
	string stringBuffer;
	vector<char> buffer;
	int lexemeBegin;
	int forward;
	int line;

	string GetNextWord();

	bool isEndOfFile(char ch);
	bool isEmptyCharacter(char ch);
	bool isDigitOrChar(char ch);
	bool isSpecialCharacter(char ch);

	TokenRecognizer tokenRecognizer;
};

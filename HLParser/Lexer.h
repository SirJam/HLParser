#pragma once

#include "Token.h"
#include "TokenRecognizer.h"
#include "Symbol.h"
#include <iostream>
#include <fstream>
#include "Sender.h"

class Lexer
{
public:
	Lexer(const char *fileName, vector<Symbol> const& symbolTable, Sender *sender);

	Token nextToken();
	bool nextTokenExists();

private:
	string stringBuffer;
	vector<char> buffer;
	int lexemeBegin;
	int forward;
	int line;
	Sender *sender;

	string GetNextWord();

	bool isEndOfFile(char ch);
	bool isEmptyCharacter(char ch);
	bool isDigitOrChar(char ch);
	bool isSpecialCharacter(char ch);
	void CheckNumberOnLength(string const &str);

	TokenRecognizer tokenRecognizer;
};

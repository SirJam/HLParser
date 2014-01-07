#include "StdAfx.h"
#include "Lexer.h"

Lexer::Lexer(const char *fileName, vector<Symbol> const& symbolTable)
:scanner(fileName), tokenRecognizer(symbolTable) {}

string Lexer::readNextWord()
{
	string word = "";

	bool readingDigitOrChar = false;
	bool readingSymbols = false;

	char ch = scanner.readCharacter();
	if (isDigitOrChar(ch)) {
		readingDigitOrChar = true;
	} else {
		readingSymbols = true;
	}

	while (ch != EOF) {
		if (isEndOfFile(ch)) {
			break;
		}

		if (isSpecialCharacter(ch) && !word.length()) {
			word.push_back(ch);
			break;
		}

		if (isEmptyCharacter(ch)) {
			if (word.length()) {
				break;
			}
		}

		if (isDigitOrChar(ch) && readingDigitOrChar) {
			word.push_back(ch);
		} else if (!isDigitOrChar(ch) && readingSymbols) {
			word.push_back(ch);
		} else {
			scanner.returnCharacter();
			break;
		}

		ch = scanner.readCharacter();
	}
	return word;
}

Token Lexer::nextToken()
{
	string word = readNextWord();

	Symbol tokenType = tokenRecognizer.tokenTypeByValue(word);
	Token token = Token(tokenType, scanner.currentLine);

	return token;
}

bool Lexer::nextTokenExists()
{
	char ch = scanner.readCharacter();
	while (isEmptyCharacter(ch)) {
		ch = scanner.readCharacter();
	}
	scanner.returnCharacter();

	return ch != EOF;
}

bool Lexer::isEndOfFile(char ch)
{
	return ch == EOF;
}

bool Lexer::isEmptyCharacter(char ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t';
}

bool Lexer::isDigitOrChar(char ch)
{
	return isdigit(ch) || isalpha(ch) || ch == '_';
}

bool Lexer::isSpecialCharacter(char ch)
{
	char nextCh = scanner.peekCharacter();
	if (ch == '(' || ch == ')' ||
		ch == '{' || ch == '}' || 
		ch == '[' || ch == ']' || 
		ch == ';' || ch == '"') 
	{
		return true;
	} 
	else 
	{
		if (ch == '-' || ch == '!') 
		{
			return (nextCh != '=');
		}
		else 
		{
			return false;
		}
	}
}

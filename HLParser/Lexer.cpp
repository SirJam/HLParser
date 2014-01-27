#include "StdAfx.h"
#include "Lexer.h"
#include "ErrorHandler.h"

Lexer::Lexer(const char *fileName, vector<Symbol> const& symbolTable, Sender *sender)
:tokenRecognizer(symbolTable),
sender(sender)
{
	char ch;
	char ch1;
	bool mulcom = false;
	bool sincom = false;
	ifstream input(fileName);

	lexemeBegin = 0;
	forward = 0;
	stringBuffer = "";
	line = 0;
	ch1 = input.get();
	ch = ch1;
	ch1 = input.get();
	while (!input.eof()) {
		if ((ch == '/') && (ch1 == '*')) {
			mulcom = true;
			ch1 = input.get();
			if (!input.eof()){
				ch = ch1;
				ch1 = input.get();
			}
			continue;
		}
		if ((ch == '/') && (ch1 == '/')) {
			sincom = true;
			ch1 = input.get();
			if (!input.eof()){
				ch = ch1;
				ch1 = input.get();
			}
			continue;
		}
		if ((ch == '\n') && (sincom == true)){
			sincom = false;
		}
		if ((ch == '*') && (ch1 == '/')){
			if (mulcom == true) {
				mulcom = false;
				ch1 = input.get();
				if (!input.eof()){
					ch = ch1;
					ch1 = input.get();
				}
				continue;
			}
			else{
				return;
			}
		}
		if ((mulcom == false) && (sincom == false)) {
			buffer.push_back(ch);
		}
		ch = ch1;
		ch1 = input.get();
	}
	buffer.push_back(ch);
	buffer.push_back(ch1);
}

string Lexer::GetNextWord()
{
	string stringBuffer = "";

	bool readingDigitOrChar = false;
	bool readingSymbols = false;

	char ch = buffer[lexemeBegin];
	if (ch == '\n') {
		line++;
	}
	if (isDigitOrChar(ch)) {
		readingDigitOrChar = true;
	}
	else {
		readingSymbols = true;
	}

	while (ch != EOF) {
		if (isEndOfFile(ch)) {
			break;
		}

		if (isSpecialCharacter(ch) && (stringBuffer.length() == 0)) {
			stringBuffer.push_back(ch);
			break;
		}

		if (isEmptyCharacter(ch)) {
			if (stringBuffer.length() != 0) {
				break;
			}
		}

		if (isDigitOrChar(ch) && readingDigitOrChar) {
			stringBuffer.push_back(ch);
		}
		else if (!isDigitOrChar(ch) && readingSymbols) {
			stringBuffer.push_back(ch);
		}
		else {
			if (lexemeBegin > 0)
			{
				lexemeBegin--;
				forward--;
			}
			char returnedChar = buffer[forward];
			if (returnedChar == '\n') {
				line--;
			}
			break;
		}

		lexemeBegin = forward;
		forward++;
		ch = buffer[lexemeBegin];
		if (ch == '\n') {
			line++;
		}
	}
	return stringBuffer;
}

Token Lexer::nextToken()
{
	string stringBuffer = GetNextWord();
	CheckNumberOnLength(stringBuffer);
	Symbol tokenType = tokenRecognizer.TokenTypeByTokensValue(stringBuffer);
	Token token = Token(tokenType, stringBuffer, line);

	return token;
}

void Lexer::CheckNumberOnLength(string const &str)
{
	bool isNumber = true;
	if (str.size() && isdigit(str.front()))
	{
		for (char ch : str)
		{
			isNumber = isdigit(ch);
		}

		if (isNumber && str.size() > 9) 
		{
			string message = ErrorHandler::FailedWithMaxLengthOfNumber(str);
			sender->SendMessageWithDescription(line, message, true);
		}
	}
}

bool Lexer::nextTokenExists()
{
	lexemeBegin = forward;
	forward++;
	char ch = buffer[lexemeBegin];
	if (ch == '\n') {
		line++;
		lexemeBegin = forward;
		forward++;
		ch = buffer[lexemeBegin];
	}
	while (isEmptyCharacter(ch)) {
		lexemeBegin = forward;
		forward++;
		ch = buffer[lexemeBegin];
		if (ch == '\n') {
			line++;
		}
	}

	char returnedChar = buffer[lexemeBegin];
	if (returnedChar == '\n') {
		line--;
	}

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
	char nextCh = buffer[forward];
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

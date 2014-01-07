#include "StdAfx.h"
#include "Scanner.h"
#include "ErrorHandler.h"

Scanner::~Scanner()
{
	fileStream.close();
}

Scanner::Scanner(const char *fileName)
:currentLine(1)
{
	fileStream.open(fileName, std::ios::in);
	if (!fileStream) {
		ErrorHandler::failWithErrorCode(001, fileName, 0);
	}
}

char Scanner::readNextStreamCharacter()
{
	char ch = fileStream.get();

	if (ch == '\n') {
		currentLine++;
	}

	return ch;
}

char Scanner::readCharacter()
{
	char ch = readNextStreamCharacter();
	
	ch = skipIfMultiLineComment(ch);
	ch = skipIfSingleLineComment(ch);

	return ch;
}

void Scanner::returnCharacter()
{
	fileStream.unget();
	char returnedChar = fileStream.peek();
	if (returnedChar == '\n') {
		currentLine--;
	}
}

char Scanner::peekCharacter()
{
	return fileStream.peek();
}

char Scanner::skipIfMultiLineComment(char ch)
{
	bool isReadingMultiLineComment = false;

	if (ch == '*' &&  fileStream.peek() == '/') {
		ErrorHandler::failWithErrorCode(002, "", currentLine);
	}

	if (ch == '/' && fileStream.peek() == '*') {
		isReadingMultiLineComment = true;
		ch = readNextStreamCharacter();
	}

	while (isReadingMultiLineComment) {
		ch = readNextStreamCharacter();
		if (ch == '*' && fileStream.peek() == '/') {
			isReadingMultiLineComment = false;
			readNextStreamCharacter();
			ch = readNextStreamCharacter();
		}

		if (fileStream.eof() && isReadingMultiLineComment) {
			ErrorHandler::failWithErrorCode(002, "", currentLine);
		}
	}

	return ch;
}

char Scanner::skipIfSingleLineComment(char ch)
{
	bool isReadingSingleLineComment = false;

	if (ch == '/' && fileStream.peek() == '/') {
		isReadingSingleLineComment = true;
	}

	while (isReadingSingleLineComment) {
		if (ch == '\n') {
			isReadingSingleLineComment = false;
		}
		ch = readNextStreamCharacter();
	}

	return ch;
}

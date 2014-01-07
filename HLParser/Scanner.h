#pragma once

#include <fstream>

using std::ifstream;

class Scanner
{
public:
	~Scanner();
	Scanner(const char *fileName);
	char readCharacter();
	void returnCharacter();

	char peekCharacter();

	int currentLine;
private:
	ifstream fileStream;

	char skipIfMultiLineComment(char ch);
	char skipIfSingleLineComment(char ch);

	char readNextStreamCharacter();
};

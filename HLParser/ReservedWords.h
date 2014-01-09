#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define YES true
#define NO false

class ReservedWords
{
public:

	static bool IsReservedWord(string word)
	{
		vector<string> reservedWords;
		reservedWords.push_back("int");
		reservedWords.push_back("float");
		reservedWords.push_back("char");
		reservedWords.push_back("bool");
		reservedWords.push_back("main");
		reservedWords.push_back("if");
		reservedWords.push_back("else");
		reservedWords.push_back("while");
		reservedWords.push_back("write");
		reservedWords.push_back("read");
		reservedWords.push_back("TRUE");
		reservedWords.push_back("FALSE");
		reservedWords.push_back("return");

		for (string reservedWord : reservedWords) {
			if (reservedWord == word)
			{
				reservedWords.clear();
				return true;
			}
		}
		reservedWords.clear();
		return false;
	}
};
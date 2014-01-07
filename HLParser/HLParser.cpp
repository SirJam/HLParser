// HLParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "GrammarInfoParser.h"
#include "Parser.h"

int main(int argc, char * argv[])
{
	char *inputFileName = "input.txt";

	Parser parser;

	Lexer lexer(inputFileName, *parser.m_symbolsTable);
	while (lexer.nextTokenExists()) {
		Token token = lexer.nextToken();
		parser.getNextToken(token);

		while (parser.lastAction == 2) {
			parser.getNextToken(token);
		}
	}

	Token lastToken = parser.createTokenEOF();
	parser.getNextToken(lastToken);

	while (parser.lastAction != 4) {
		parser.getNextToken(lastToken);
	}

	return 0;
}


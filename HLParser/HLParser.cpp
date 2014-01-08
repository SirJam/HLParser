// HLParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "GrammarInfoParser.h"
#include "Parser.h" 
#include "tinyxml.h"

void dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		dump_to_stdout(&doc); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}

int main(int argc, char * argv[])
{
	char *inputFileName = "input.txt";
	dump_to_stdout("HLGrammar.xml");
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


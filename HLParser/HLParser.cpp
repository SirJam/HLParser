// HLParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "Parser.h" 
#include "Generator.h"

int main(int argc, char * argv[])
{
	char *inputFileName = "input.txt";
	
	Generator generator;
	Parser parser(&generator);	

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

	generator.MakeASM("input.asm");
	ostringstream paramStream;
	paramStream << "GoAsm.exe ";
	paramStream << "input.asm";
	system(paramStream.str().c_str());
	
	ostringstream paramStream1;
	paramStream1 << "GoLink.exe /console ";
	paramStream1 << "input.obj";
	paramStream1 << " kernel32.dll";
	system(paramStream1.str().c_str());

	return 0;
}


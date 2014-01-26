// HLParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "Parser.h" 
#include "Generator.h"

int main(int argc, char * argv[])
{
	bool a;
	a = true - true;
	cout << a << endl;

	char *inputFileName = "input.txt";
	
	Generator generator;
	Parser parser(&generator);	

	Lexer lexer(inputFileName, *parser.m_symbolsTable);
	while (lexer.nextTokenExists()) {
		Token token = lexer.nextToken();
		parser.GetNextToken(token);

		while (parser.m_lastAction == 2) {
			parser.GetNextToken(token);
		}
	}

	Token lastToken = parser.CreateTokenEOF();
	parser.GetNextToken(lastToken);

	while (parser.m_lastAction != 4) {
		parser.GetNextToken(lastToken);
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


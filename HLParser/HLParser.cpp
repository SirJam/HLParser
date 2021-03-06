// HLParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "Parser.h" 
#include "Generator.h"
#include "Sender.h"

int main(int argc, char * argv[])
{
	char *inputFileName = argv[1];
	
	Sender *sender = new Sender();
	Generator generator;
	Parser parser(&generator, sender);	

	Lexer lexer(inputFileName, *parser.m_symbolsTable, sender);
	sender->SendMessageWithDescription(0, "Scanning characters completed!", false);
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
	sender->SendMessageWithDescription(0, "Parsing completed!", false);

	string name = inputFileName;
	name += ".asm";

	generator.MakeASM(name);
	ostringstream paramStream;
	paramStream << "GoAsm.exe ";
	paramStream << name;
	system(paramStream.str().c_str());

	sender->SendMessageWithDescription(0, "'" + name + "'" + "has been maked succesfully!", false);
	
	name = inputFileName;
	name += ".obj";

	ostringstream paramStream1;
	paramStream1 << "GoLink.exe /console ";
	paramStream1 << name;
	paramStream1 << " kernel32.dll";
	system(paramStream1.str().c_str());

	sender->SendMessageWithDescription(0, "'" + name + "'" + "has been maked succesfully!", false);
	sender->SendMessageWithDescription(0, "Running application...", false);

	return 0;
}


#include "StdAfx.h"
#include "Generator.h"
#include <fstream>

Generator::Generator()
{
	createProgramStart();
}

void Generator::createASMFile(string fileName)
{
	ofstream outFile;
	outFile.open(fileName);

	//outFile << dataStream.str().c_str();
	outFile << programStream.str().c_str();
	outFile << procedureStream.str().c_str();
	outFile.close();
}


void Generator::createProgramStart()
{
	ostringstream streamOfProgram;
	ostringstream streamOfData;

	streamOfProgram << "\nCODE SECTION\n";
	streamOfProgram << "START:\n";
	streamOfProgram << "PUSH EBP\n"; // Saving previous stackbase-pointer register
	streamOfProgram << "MOV EBP, ESP\n\n";

	streamOfData << "DATA SECTION\n";

	programStream << streamOfProgram.str();
	dataStream << streamOfData.str();
}

void Generator::createProgramEnd()
{
	ostringstream stream;

	stream << "MOV EAX, 0\n"; // to indicate successful end of program
	stream << "RET\n\n";

	programStream << stream.str();
}

void Generator::createVariable(string name, int const value, bool const isArray = false, int const arrSize = 0)
{
	ostringstream stream;

	stream << name << "dw";
	if (isArray)
	{
		stream << arrSize << "dup (" << value << ")\n";
	}
	else 
	{
		stream << value << ")\n\n";
	}

	programStream << stream.str();
}

void Generator::createAddOperation()
{
	ostringstream stream;

	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "ADD EAX, EDX\n";
	stream << "PUSH EAX\n\n";

	programStream << stream.str();
}

void Generator::createSubstractOperation()
{
	ostringstream stream;

	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "SUB EDX, EAX\n";
	stream << "PUSH EDX\n\n";

	programStream << stream.str();
}

void Generator::createMultiplyOperation()
{
	ostringstream stream;

	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "IMUL EAX, EDX\n";
	stream << "PUSH EAX\n";

	programStream << stream.str();
}

void Generator::createDivideOperation()
{
	ostringstream stream;

	stream << "XOR EDX, EDX\n";
	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CDQ\n";
	stream << "IDIV EBX\n";
	stream << "PUSH EAX\n";

	programStream << stream.str();
}
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

	outFile << dataStream.str().c_str();
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

void Generator::createProgramEnd(int const varSpace)
{
	ostringstream stream;

	stream << "ADD ESP, " << varSpace << "\n";
	stream << "MOV ESP, EBP\n";
	stream << "POP EBP\n";
	stream << "MOV EAX, 0\n";
	stream << "RET\n\n";

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
	stream << "PUSH EAX\n\n";

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
	stream << "PUSH EAX\n\n";

	programStream << stream.str();
}

/*void Generator::createIntConstant(int num)
{
	ostringstream stream;

	stream << "PUSH " << num << "\n";

	programStream << stream.str();
}*/

/*void Generator::createIntVariable(int const offset, bool const isInArray, int const typeSize)
{
	addReadVariable(offset, isInArray, typeSize);

	ostringstream stream;

	stream << "PUSH EAX\n";

	programStream << stream.str();
}*/



void Generator::createVariableSpace(int const space)
{
	ostringstream stream;

	stream << "SUB ESP, " << space << "\n\n";

	programStream << stream.str();
}

void Generator::createIntVariable(int const offset, bool const isInArray, int const typeSize)
{
	addReadVariable(offset, isInArray, typeSize);

	ostringstream stream;

	stream << "PUSH EAX\n\n";

	programStream << stream.str();
}

void Generator::addReadVariable(int const offset, bool const isInArray, int const typeSize)
{
	ostringstream stream;

	if (isInArray) {
		stream << "POP EDX\n";
		stream << "MOV EAX, " << typeSize << "\n";
		stream << "IMUL EAX, EDX\n";

		stream << "MOV EDX, " << offset << "\n";
		stream << "SUB EDX, EAX\n";
		stream << "NEG EDX\n";
		stream << "MOV EAX, [EBP + EDX]\n";
	}
	else {
		stream << "MOV EAX, [EBP - " << offset << "]\n";
	}

	programStream << stream.str(); 
}

void Generator::createIntConstant(int num)
{
	ostringstream stream;

	stream << "PUSH " << num << "\n\n";

	programStream << stream.str();
}

void Generator::createAssignmentOperation(int const offset, bool const isInArray, int const typeSize)
{
	ostringstream stream;

	stream << "POP EAX\n";

	programStream << stream.str();

	addWriteVariable(offset, isInArray, typeSize);
}

void Generator::addWriteVariable(int const offset, bool const isInArray, int const typeSize)
{
	ostringstream stream;

	if (isInArray) {
		stream << "POP EDX\n";
		stream << "MOV EBX, " << typeSize << "\n";
		stream << "IMUL EBX, EDX\n";

		stream << "MOV EDX, " << offset << "\n";
		stream << "SUB EDX, EBX\n";
		stream << "NEG EDX\n";
		stream << "MOV [EBP + EDX], EAX\n\n";
	}
	else {
		stream << "MOV [EBP - " << offset << "], EAX\n\n";
	}

	programStream << stream.str();
}
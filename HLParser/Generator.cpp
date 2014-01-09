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

void Generator::addClearBuffer()
{
	ostringstream streamOfProcedures;

	streamOfProcedures << "CLEAR_BUFFER:\n";
	streamOfProcedures << "MOV ECX, 11\n";
	streamOfProcedures << "MOV EBX, 0x30\n";
	streamOfProcedures << "LEA EDX, ADDR BUFFER\n";

	streamOfProcedures << "CB_LOOP:\n";

	streamOfProcedures << "MOV B [EDX + ECX], BL\n";

	streamOfProcedures << "DEC ECX\n";
	streamOfProcedures << "CMP ECX, 0\n";
	streamOfProcedures << "JNE CB_LOOP\n";

	streamOfProcedures << "RET\n";

	procedureStream << streamOfProcedures.str();
}

void Generator::createPrintInteger(bool const addNewLine)
{
	ostringstream stream;

	stream << "POP EAX\n";

	stream << "CALL CLEAR_BUFFER\n";
	stream << "CALL FILL_BUFFER\n";

	if (addNewLine) {
		stream << "LEA EDX, ADDR BUFFER\n";
		stream << "MOV EBX, [CONST_10]\n";
		stream << "MOV B [EDX + EBX], 0AH\n"; // Adding new line character
	}

	stream << "MOV EBX, EAX\n";

	stream << "PUSH CONSOLE_OUT_HANDLE_CODE\n";
	stream << "CALL GetStdHandle\n";

	stream << "PUSH 0\n";
	stream << "PUSH ADDR RCKEEP\n";

	stream << "MOV ECX, [CONST_10]\n";
	stream << "SUB ECX, EBX\n"; // The length of string depends on the number of cycles in FILL_BUFFER
	if (addNewLine) {
		stream << "INC ECX\n"; // Adding extra length for new line
	}
	stream << "PUSH ECX\n";

	stream << "MOV ECX, ADDR BUFFER\n";
	stream << "ADD ECX, EBX\n"; // Address of string with offest, which removes leading zeros
	stream << "PUSH ECX\n";

	stream << "PUSH EAX\n";
	stream << "CALL WriteFile\n";

	programStream << stream.str();
}
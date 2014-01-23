#include "StdAfx.h"
#include "Generator.h"
#include <fstream>

Generator::Generator()
:isPrintAdded(false), 
isInputOutputDataAdded(false), 
isReadAdded(false),
labelsCount(0)
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

void Generator::createVariableSpace(int const space)
{
	ostringstream stream;
	stream << "SUB ESP, " << space << "\n\n";
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

void Generator::createIntConstant(int num)
{
	ostringstream stream;
	stream << "PUSH " << num << "\n\n";
	programStream << stream.str();
}

void Generator::createIntVariable(int const offset, string const type, int x)
{
	addReadVariable(offset, type, x);
	ostringstream stream;
	stream << "PUSH EAX\n\n";
	programStream << stream.str();
}

void Generator::createAssignmentOperation(int const offset, string const type, int x)
{
	addWriteVariable(offset, type, x);
}

void Generator::addWriteVariable(int const offset, string const type, int xDim)
{
	ostringstream stream;

	if (type == "int_array" || type == "bool_array") {
		stream << "POP EDX\n";
		stream << "MOV EAX, " << 4 << "\n";
		stream << "IMUL EAX, EDX\n";
		
		stream << "MOV EDX, " << offset << "\n";
		stream << "SUB EDX, EAX\n";
		stream << "NEG EDX\n";
		stream << "POP EAX\n";
		stream << "MOV [EBP + EDX], EAX\n";
	}
	else if (type == "int_double_array" || type == "bool_double_array") {		
		stream << "POP EDX\n";
		stream << "POP EAX\n";
		stream << "IMUL EDX, " << xDim << "\n";		
		stream << "ADD EAX, EDX\n";
		stream << "IMUL EAX, 4\n";

		stream << "MOV EDX, " << offset << "\n";
		stream << "SUB EDX, EAX\n";
		stream << "NEG EDX\n";
		stream << "POP EAX\n";
		stream << "MOV [EBP + EDX], EAX\n";
	}
	else {
		stream << "POP EAX\n";
		stream << "MOV [EBP - " << offset << "], EAX\n\n";
	}

	programStream << stream.str();
}

void Generator::addReadVariable(int const offset, string const type, int xDim) //read value from stack to EAX
{
	ostringstream stream;

	if (type == "int_array" || type == "bool_array") {
		stream << "POP EDX\n";
		stream << "MOV EAX, " << 4 << "\n";
		stream << "IMUL EAX, EDX\n";
		
		stream << "MOV EDX, " << offset << "\n";
		stream << "SUB EDX, EAX\n";
		stream << "NEG EDX\n";
		stream << "MOV EAX, [EBP + EDX]\n";
	}
	else if (type == "int_double_array" || type == "bool_double_array") {
		stream << "POP EDX\n";
		stream << "POP EAX\n";
		stream << "IMUL EDX, " << xDim << "\n";		
		stream << "ADD EAX, EDX\n";
		stream << "IMUL EAX, 4\n";

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

void Generator::createPrintInteger()
{
	if (!isPrintAdded) {
		addPrintFunctionality();
		isPrintAdded = true;
	}

	ostringstream stream;

	stream << "POP EAX\n";

	stream << "CALL CLEAR_BUFFER\n";
	stream << "CALL FILL_BUFFER\n";

	//if (addNewLine) {
	//	stream << "LEA EDX, ADDR BUFFER\n";
	//	stream << "MOV EBX, [CONST_10]\n";
	//	stream << "MOV B [EDX + EBX], 0AH\n"; // Adding new line character
	//}

	stream << "MOV EBX, EAX\n";

	stream << "PUSH CONSOLE_OUT_HANDLE_CODE\n";
	stream << "CALL GetStdHandle\n";

	stream << "PUSH 0\n";
	stream << "PUSH ADDR RCKEEP\n";

	stream << "MOV ECX, [CONST_10]\n";
	stream << "SUB ECX, EBX\n"; // The length of string depends on the number of cycles in FILL_BUFFER
	//if (addNewLine) {
	//	stream << "INC ECX\n"; // Adding extra length for new line
	//}
	stream << "PUSH ECX\n";

	stream << "MOV ECX, ADDR BUFFER\n";
	stream << "ADD ECX, EBX\n"; // Address of string with offest, which removes leading zeros
	stream << "PUSH ECX\n";

	stream << "PUSH EAX\n";
	stream << "CALL WriteFile\n\n";

	programStream << stream.str();
}

void Generator::addPrintFunctionality()
{
	ostringstream streamOfData;
	ostringstream streamOfProcedures;

	streamOfData << "CONSOLE_OUT_HANDLE_CODE EQU -11\n";

	if (!isInputOutputDataAdded) {
		streamOfData << "RCKEEP DD 0\n";
		streamOfData << "CONST_10 DD 10\n";
		streamOfData << "BUFFER DB 12 DUP ('0')\n"; // Extra space for new line and negative sign characters
		addClearBuffer();
		isInputOutputDataAdded = true;
	}

	streamOfProcedures << "FILL_BUFFER:\n"; // filling the string buffer with number in EAX

	streamOfProcedures << "MOV ECX, [CONST_10]\n"; // number of cycles

	streamOfProcedures << "PUSH EAX\n";
	streamOfProcedures << "TEST EAX, EAX\n";
	streamOfProcedures << "JNS >FB_LOOP\n"; // if number is negative, make it positive and add negative sign
	streamOfProcedures << "NEG EAX\n";

	streamOfProcedures << "FB_LOOP:\n";
	streamOfProcedures << "XOR EDX, EDX\n";
	streamOfProcedures << "MOV EBX, [CONST_10]\n";
	streamOfProcedures << "DIV EBX\n";
	streamOfProcedures << "ADD EDX, 0x30\n"; // creating ASCII number from integer number

	streamOfProcedures << "PUSH EAX\n";
	streamOfProcedures << "LEA EAX, ADDR BUFFER\n"; 
	streamOfProcedures << "MOV [EAX + ECX - 1], DL\n"; // writing number in string buffer
	streamOfProcedures << "POP EAX\n";

	streamOfProcedures << "DEC ECX\n";

	streamOfProcedures << "OR EAX, EAX\n"; // cycle can end if the quotient is zero
	streamOfProcedures << "JZ >FB_LOOP_END\n";

	streamOfProcedures << "JNE FB_LOOP\n";

	streamOfProcedures << "FB_LOOP_END:\n";
	streamOfProcedures << "POP EBX\n";
	streamOfProcedures << "TEST EBX, EBX\n";
	streamOfProcedures << "JNS >FB_END\n";
	streamOfProcedures << "LEA EAX, ADDR BUFFER\n"; 
	streamOfProcedures << "MOV B [EAX + ECX - 1], 2DH\n"; // adding negative sign to string
	streamOfProcedures << "DEC ECX\n";

	streamOfProcedures << "FB_END:\n";
	streamOfProcedures << "MOV EAX, ECX\n"; // saving number of remaining cycles
	streamOfProcedures << "RET\n";

	streamOfProcedures << "STRING_LENGTH:\n"; // returns length of string, whose address is in EAX. The length is later stored in EAX
	streamOfProcedures << "MOV ECX, -1\n";
	streamOfProcedures << "MOV EBX, EAX\n";
	streamOfProcedures << "LEA EAX, EBX\n";
	streamOfProcedures << "SL_LOOP:\n";
	streamOfProcedures << "INC ECX\n";

	streamOfProcedures << "XOR EBX, EBX\n";
	streamOfProcedures << "MOV BL, [EAX + ECX]\n";
	streamOfProcedures << "CMP BL, 0x0\n";
	streamOfProcedures << "JNE <SL_LOOP\n";
	streamOfProcedures << "MOV EAX, ECX\n";
	streamOfProcedures << "RET\n";

	dataStream << streamOfData.str();
	procedureStream << streamOfProcedures.str();
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

void Generator::createRelExpression(const string& rel)
{
	ostringstream stream;

	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CMP EAX, EBX\n";
	
	string command;
	if (rel == "<=") {
		command = "JLE";
	} else if (rel == "<") {
		command = "JL";
	} else if (rel == ">") {
		command = "JG";
	} else if (rel == ">=") {
		command = "JGE";
	} else if (rel == "==") {
		command = "JE";
	} else {
		command = "JNE";
	}

	string trueLabel = getLabelName();
	string endLabel = getLabelName();
	cout << trueLabel << " " << endLabel << std::endl;
	command += " >" + trueLabel + "\n";

	stream << command;
	stream << "MOV EAX, 0\n";
	stream << ("JMP >" + endLabel + "\n");
	stream << (trueLabel) + ":\n";
	stream << "MOV EAX, 1\n";
	stream << (endLabel) + ":\n";
	stream << "PUSH EAX\n";
	
	programStream << stream.str();
}

string Generator::getLabelName()
{
	string label = "@label" + std::to_string(static_cast<long long>(labelsCount));
	labelsCount++;
	return label;
}

void Generator::applyAndExpression()
{
	ostringstream stream;
	
	string elseLabel = getLabelName();
	string endLabel = getLabelName();

	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + elseLabel + "\n");
	stream << "CMP EBX, 0\n";
	stream << ("JE >" + elseLabel + "\n");
	stream << "PUSH 1\n";
	stream << ("JMP >" + endLabel + "\n");
	stream << (elseLabel + ":\n");
	stream << "PUSH 0\n";
	stream << (endLabel + ":\n");

	programStream << stream.str();
}

void Generator::applyOrExpression()
{
	ostringstream stream;
	
	string trueLabel = getLabelName();
	string endLabel = getLabelName();

	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "OR EAX, EBX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + trueLabel + "\n");
	stream << "PUSH 1\n";
	stream << ("JMP >" + endLabel + "\n");
	stream << (trueLabel + ":\n");
	stream << "PUSH 0\n";
	stream << (endLabel + ":\n");

	programStream << stream.str();
}

void Generator::writeSomething()
{
	ostringstream stream;
	stream << "something\n";
	programStream << stream.str();
}
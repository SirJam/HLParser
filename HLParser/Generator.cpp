#include "StdAfx.h"
/*#include "Generator.h"
#include <fstream>

Generator::Generator()
:isPrintAdded(false), 
isInputOutputDataAdded(false), 
isReadAdded(false), 
isMainStringBufferAdded(false), 
labelsCreated(0)
{
	createProgramStart();
}

void Generator::writeToFile(string fileName)
{
	size_t dotPosition = fileName.find_first_of(".");
	if (dotPosition != string::npos) {
		fileName = fileName.substr(0, dotPosition);
	}
	cleanfileName = fileName;

	ofstream outFile;
	outFile.open(fileName + ".asm");

	outFile << dataStream.str().c_str();
	outFile << programStream.str().c_str();
	outFile << procedureStream.str().c_str();
	outFile.close();
}

void Generator::writeToConsole()
{
	cout << "Assembly program:\n";
	cout << "====================\n";
	cout << dataStream.str();
	cout << programStream.str();
	cout << procedureStream.str();
	cout << "====================\n";
}

void Generator::createExecutable()
{
	ostringstream paramStream;
	paramStream << "GoLink.exe /console ";
	paramStream << cleanfileName << ".obj";
	paramStream << " kernel32.dll";
	system(paramStream.str().c_str());
}

string Generator::getExecutableName()
{
	return (cleanfileName + ".exe");
}

void Generator::createObjectFile()
{
	ostringstream paramStream;
	paramStream << "GoAsm.exe ";
	paramStream << cleanfileName << ".asm";
	system(paramStream.str().c_str());
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

void Generator::addMainStringBuffers()
{
	ostringstream stream;

	stream << "MAIN_STRING_BUFFER DB " << stringLength << " DUP (?)\n";
	stream << "MAIN_STRING_BUFFER_2 DB " << stringLength << " DUP (?)\n";
	stream << "MAIN_STRING_BUFFER_3 DB " << stringLength << " DUP (?)\n";

	dataStream << stream.str();
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

void Generator::addReadFunctionality()
{
	ostringstream streamOfData;
	ostringstream streamOfProcedures;

	streamOfData << "CONSOLE_IN_HANDLE_CODE EQU -10\n";

	if (!isInputOutputDataAdded) {
		streamOfData << "RCKEEP DD 0\n";
		streamOfData << "CONST_10 DD 10\n";
		streamOfData << "BUFFER DB 12 DUP ('0')\n";
		addClearBuffer();
		isInputOutputDataAdded = true;
	}

	streamOfProcedures << "BUFFER_TO_INT:\n"; // converts number stored in buffer as string and saves it to EAX

	streamOfProcedures << "XOR EAX, EAX\n";

	streamOfProcedures << "MOV ECX, [RCKEEP]\n"; // RCKEEP has number of read characters
	streamOfProcedures << "SUB ECX, 2\n"; // Because buffer may have /r and /n characters
	streamOfProcedures << "MOV EDX, 1\n";

	streamOfProcedures << "BTI_LOOP:\n";

	streamOfProcedures << "PUSH EAX\n";
	streamOfProcedures << "LEA EAX, ADDR BUFFER\n";

	streamOfProcedures << "XOR EBX, EBX\n";
	streamOfProcedures << "MOV BL, [EAX + ECX - 1]\n";
	streamOfProcedures << "POP EAX\n";

	streamOfProcedures << "CMP BL, 2DH\n"; // check for negative sign
	streamOfProcedures << "JNZ >BTI_NOT_NEGATIVE_SIGN\n";
	streamOfProcedures << "CMP ECX, 1\n";
	streamOfProcedures << "JNZ >BTI_NOT_NEGATIVE_SIGN\n";
	streamOfProcedures << "NEG EAX\n";
	streamOfProcedures << "JMP >BTI_END\n";

	streamOfProcedures << "BTI_NOT_NEGATIVE_SIGN:\n";
	streamOfProcedures << "SUB EBX, 0x30\n"; // getting digit from ASCII code
	streamOfProcedures << "IMUL EBX, EDX\n";

	streamOfProcedures << "ADD EAX, EBX\n";

	streamOfProcedures << "MOV EBX, [CONST_10]\n";
	streamOfProcedures << "IMUL EDX, EBX\n";

	streamOfProcedures << "DEC ECX\n";
	streamOfProcedures << "CMP ECX, 0\n";
	streamOfProcedures << "JNE BTI_LOOP\n";
	
	streamOfProcedures << "BTI_END:\n";
	streamOfProcedures << "RET\n";

	dataStream << streamOfData.str();
	procedureStream << streamOfProcedures.str();
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
		stream << "MOV [EBP + EDX], EAX\n";
	}
	else {
		stream << "MOV [EBP - " << offset << "], EAX\n";
	}

	programStream << stream.str();
}

void Generator::createProgramStart()
{
	ostringstream streamOfProgram;
	ostringstream streamOfData;

	streamOfProgram << "\nCODE SECTION\n";
	streamOfProgram << "START:\n";
	streamOfProgram << "PUSH EBP\n"; // Saving previous stackbase-pointer register
	streamOfProgram << "MOV EBP, ESP\n";

	streamOfData << "DATA SECTION\n";

	programStream << streamOfProgram.str();
	dataStream << streamOfData.str();
}

void Generator::createProgramEnd()
{
	ostringstream stream;

	stream << "MOV ESP, EBP\n"; // Undo the carving of space for the local variables
	stream << "POP EBP\n"; 

	stream << "MOV EAX, 0\n"; // to indicate successful end of program
	stream << "RET\n\n";

	programStream << stream.str();
}

void Generator::createIntConstant(int num)
{
	ostringstream stream;

	stream << "PUSH " << num << "\n";

	programStream << stream.str();
}

void Generator::createIntVariable(int const offset, bool const isInArray, int const typeSize)
{
	addReadVariable(offset, isInArray, typeSize);

	ostringstream stream;

	stream << "PUSH EAX\n";

	programStream << stream.str();
}



void Generator::createAddOperation()
{
	ostringstream stream;

	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "ADD EAX, EDX\n";
	stream << "PUSH EAX\n";

	programStream << stream.str();
}

void Generator::createSubstractOperation()
{
	ostringstream stream;

	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "SUB EDX, EAX\n";
	stream << "PUSH EDX\n";

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

void Generator::createRemainderOperation()
{
	ostringstream stream;

	stream << "XOR EDX, EDX\n";
	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CDQ\n";
	stream << "IDIV EBX\n";
	stream << "PUSH EDX\n";

	programStream << stream.str();
}

void Generator::createAssignmentWithAddOperation(int const offset, bool const isInArray, int const typeSize)
{
	ostringstream stream;

	if (isInArray) { // We need to reverse two last elements in stack and add another position in array for later writing
		stream << "POP EAX\n";
		stream << "POP EBX\n";
		stream << "PUSH EBX\n";
		stream << "PUSH EAX\n";
		stream << "PUSH EBX\n";

		programStream << stream.str();

		stream.str("");
		stream.clear();
	}

	addReadVariable(offset, isInArray, typeSize);

	stream << "PUSH EAX\n";
	
	programStream << stream.str();

	createAddOperation();

	createAssignmentOperation(offset, isInArray, typeSize);
}

void Generator::createAssignmentWithSubstractOperation(int const offset, bool const isInArray, int const typeSize)
{
	ostringstream stream;

	if (isInArray) {
		stream << "POP EAX\n";
		stream << "POP EBX\n";
		stream << "PUSH EBX\n";
		stream << "PUSH EAX\n";
		stream << "PUSH EBX\n";

		programStream << stream.str();

		stream.str("");
		stream.clear();
	}

	addReadVariable(offset, isInArray, typeSize);


	stream << "PUSH EAX\n";
	programStream << stream.str();

	createSubstractOperation();

	stream.str("");
	stream.clear();

	stream << "POP EAX\n";
	stream << "NEG EAX\n";
	stream << "PUSH EAX\n";

	programStream << stream.str();

	createAssignmentOperation(offset, isInArray, typeSize);
}

void Generator::createAssignmentOperation(int const offset, bool const isInArray, int const typeSize)
{
	ostringstream stream;

	stream << "POP EAX\n";

	programStream << stream.str();

	addWriteVariable(offset, isInArray, typeSize);
}










void Generator::createVariableSpace(int const variableSpace)
{
	ostringstream stream;

	stream << "SUB ESP, " << variableSpace << "\n";

	programStream << stream.str();
}

void Generator::removeVariableSpace(int const variableSpace)
{
	ostringstream stream;

	stream << "ADD ESP, " << variableSpace << "\n";

	programStream << stream.str();
}

void Generator::createPrintInteger(bool const addNewLine)
{
	if (!isPrintAdded) {
		addPrintFunctionality();
		isPrintAdded = true;
	}

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

void Generator::createPrintBool(bool const addNewLine)
{	 
	if (!isMainStringBufferAdded) {
		addMainStringBuffers();
		isMainStringBufferAdded = true;
	}

	addCopyString();
	
	ostringstream stream;
	
	string trueLabel = getFreeLabelName();
	string endLabel = getFreeLabelName();
	
	stream << "POP EAX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + trueLabel + "\n");
	stream << "MOV EAX, ADDR 'true'\n";
	stream << ("JMP >" + endLabel + "\n");
	stream << (trueLabel + ":\n");
	stream << "MOV EAX, ADDR 'false'\n";
	stream << (endLabel + ":\n");

	stream << "MOV EBX, ADDR MAIN_STRING_BUFFER\n";
	stream << "CALL COPY_STRING\n";

	programStream << stream.str();

	createPrintString(addNewLine);
}

void Generator::createReadIntToVariable(int const offset, bool const isInArray, int const typeSize)
{
	if (!isReadAdded) {
		addReadFunctionality();
		isReadAdded = true;
	}

	ostringstream stream;

	stream << "CALL CLEAR_BUFFER\n";
	stream << "PUSH CONSOLE_IN_HANDLE_CODE\n";
	stream << "CALL GetStdHandle\n";
	
	stream << "PUSH 0\n";
	stream << "PUSH ADDR RCKEEP\n";
	stream << "MOV EBX, [CONST_10]\n";
	stream << "INC EBX\n";
	stream << "PUSH EBX\n";
	stream << "PUSH ADDR BUFFER\n";
	stream << "PUSH EAX\n";
	stream << "CALL ReadFile\n";

	stream << "CALL BUFFER_TO_INT\n";
	programStream << stream.str();

	addWriteVariable(offset, isInArray, typeSize);
}



void Generator::negateResult()
{
	ostringstream stream;

	stream << "POP EAX\n";
	stream << "NEG EAX\n";
	stream << "PUSH EAX\n";

	programStream << stream.str();
}

void Generator::createBoolConstant(bool boolConstant)
{
	ostringstream stream;
	int num = (boolConstant) ? 1 : 0;

	stream << "PUSH " << num << "\n";

	programStream << stream.str();
}

void Generator::createBoolVariable(int const offset, bool const isInArray, int const typeSize)
{
	addReadVariable(offset, isInArray, typeSize);

	ostringstream stream;

	stream << "PUSH EAX\n";

	programStream << stream.str();
}

void Generator::applyOrExpression()
{
	ostringstream stream;
	
	string trueLabel = getFreeLabelName();
	string endLabel = getFreeLabelName();

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

void Generator::applyAndExpression()
{
	ostringstream stream;
	
	string elseLabel = getFreeLabelName();
	string endLabel = getFreeLabelName();

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

void Generator::createExpressionInversion()
{
	ostringstream stream;
	
	stream << "POP EAX\n";
	stream << "XOR EAX, 1\n";
	stream << "PUSH EAX\n";

	programStream << stream.str();
}
	
void Generator::createRelExpression(const string& relOperator)
{
	ostringstream stream;

	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CMP EAX, EBX\n";
	
	string command;
	if (relOperator == "<=") {
		command = "JLE";
	} else if (relOperator == "<") {
		command = "JL";
	} else if (relOperator == ">") {
		command = "JG";
	} else if (relOperator == ">=") {
		command = "JGE";
	} else if (relOperator == "==") {
		command = "JE";
	} else {// relOperator == "!="
		command = "JNE";
	}

	string trueLabel = getFreeLabelName();
	string endLabel = getFreeLabelName();
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

void Generator::createIfExpressionStartPart(string &falseLabel)
{
	ostringstream stream;

	falseLabel = getFreeLabelName();
	
	stream << "POP EAX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + falseLabel + "\n");

	programStream << stream.str();
}

void Generator::createIfExpressionEndPart(string falseLabel)
{
	ostringstream stream;
		
	stream << (falseLabel + ":\n");

	programStream << stream.str();
}

string Generator::getFreeLabelName()
{
	string newLabel = "@label" + std::to_string(static_cast<long long>(++labelsCreated));
	return newLabel;
}

void Generator::createCthulhuFunction()
{
	if (!isPrintAdded) {
		addPrintFunctionality();
		isPrintAdded = true;
	}

	ostringstream stream;

	stream << "PUSH CONSOLE_OUT_HANDLE_CODE\n";
	stream << "CALL GetStdHandle\n";
	stream << "MOV EBX, EAX\n";
	
	stream << "MOV EDX, 20H\n";
	stream << "OR EDX, 07H\n";

	stream << "PUSH EDX\n";
	stream << "PUSH EBX\n";
	stream << "CALL SetConsoleTextAttribute\n";

	stream << "PUSH 'Space reserved'\n";
	stream << "CALL SetConsoleTitleA\n";
	
	programStream << stream.str();
}*/
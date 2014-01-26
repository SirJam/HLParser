#include "StdAfx.h"
#include "Generator.h"
#include <fstream>

Generator::Generator()
:m_isPrintAdded(false), 
m_isInputOutputDataAdded(false), 
m_isReadAdded(false),
m_labelsCount(0)
{
	WriteProgramHead();
}

void Generator::MakeASM(string fileName)
{
	ofstream outFile;
	outFile.open(fileName);
	outFile << m_data.str().c_str();
	outFile << m_program.str().c_str();
	outFile << m_function.str().c_str();
	outFile.close();
}


void Generator::WriteProgramHead()
{
	ostringstream streamOfProgram;
	ostringstream streamOfData;
	streamOfProgram << "\nCODE SECTION\n";
	streamOfProgram << "START:\n";
	streamOfProgram << "PUSH EBP\n"; // Saving previous stackbase-pointer register
	streamOfProgram << "MOV EBP, ESP\n\n";
	streamOfData << "DATA SECTION\n";
	m_program << streamOfProgram.str();
	m_data << streamOfData.str();
}

void Generator::WriteProgramEnd(int const varSpace)
{
	ostringstream stream;
	stream << "ADD ESP, " << varSpace << "\n";
	stream << "MOV ESP, EBP\n";
	stream << "POP EBP\n";
	stream << "MOV EAX, 0\n";
	stream << "RET\n\n";
	m_program << stream.str();
}

void Generator::AllocateSpace(int const space)
{
	ostringstream stream;
	stream << "SUB ESP, " << space << "\n\n";
	m_program << stream.str();
}

void Generator::AddToAsm()
{
	ostringstream stream;
	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "ADD EAX, EDX\n";
	stream << "PUSH EAX\n\n";
	m_program << stream.str();
}

void Generator::SubToAsm()
{
	ostringstream stream;
	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "SUB EDX, EAX\n";
	stream << "PUSH EDX\n\n";
	m_program << stream.str();
}

void Generator::MulToAsm()
{
	ostringstream stream;
	stream << "POP EAX\n";
	stream << "POP EDX\n";
	stream << "IMUL EAX, EDX\n";
	stream << "PUSH EAX\n\n";
	m_program << stream.str();
}

void Generator::DivToAsm()
{
	ostringstream stream;
	stream << "XOR EDX, EDX\n";
	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CDQ\n";
	stream << "IDIV EBX\n";
	stream << "PUSH EAX\n\n";
	m_program << stream.str();
}

void Generator::ModuloToAsm()
{
	ostringstream stream;
	stream << "XOR EDX, EDX\n";
	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "CDQ\n";
	stream << "IDIV EBX\n";
	stream << "PUSH EDX\n";
	m_program << stream.str();
}

void Generator::WriteIntConst(int num)
{
	ostringstream stream;
	stream << "PUSH " << num << "\n\n";
	m_program << stream.str();
}

void Generator::WriteIntVar(int const offset, string const type, int x)
{
	AddReadVariable(offset, type, x);
	ostringstream stream;
	stream << "PUSH EAX\n\n";
	m_program << stream.str();
}

void Generator::WriteAssignment(int const offset, string const type, int x)
{
	if (type.find("bool") != string::npos)
	{
		//change EAX value to 0 or 1
		string l1 = GetLabelName();
		string l2 = GetLabelName();
		ostringstream stream;
		stream << "POP EAX\n";
		stream << "CMP EAX, 1\n";
		stream << "JGE >" << l1 << "\n";
		stream << "MOV EAX, 0\n";
		stream << "JMP >" << l2 << "\n";
		stream << l1 << ":\n";
		stream << "MOV EAX, 1\n";
		stream << l2 << ":\n";
		stream << "PUSH EAX\n";
		m_program << stream.str();
	}
	AddWriteVariable(offset, type, x);
}

void Generator::HandleArray(int offset)
{
	string l1 = GetLabelName();
	string e1 = GetLabelName();
	ostringstream stream;

	stream << "POP EDX\n";
	stream << "MOV EAX, " << 4 << "\n";
	stream << "IMUL EAX, EDX\n";
		
	stream << "MOV EDX, " << offset << "\n";
	stream << "SUB EDX, EAX\n";

	//EDX must be <= offset && > 0
	stream << "CMP EDX, " << offset << "\n";
	stream << "JG >" << e1 << "\n";
	stream << "CMP EDX, 0\n";
	stream << "JLE >" << e1 << "\n";
	stream << "JMP >" << l1 << "\n";
	stream << e1 << ":" << "\n";
	stream << "HLT\n";
	stream << l1 << ":" << "\n";

	stream << "NEG EDX\n";

	m_program << stream.str();
}

void Generator::HandleDoubleArray(int offset, int xDim)
{
	string l1 = GetLabelName();
	string e1 = GetLabelName();
	string l2 = GetLabelName();
	string e2 = GetLabelName();
	ostringstream stream;

	stream << "POP EDX\n"; //y
	stream << "POP EAX\n"; //x

	//x must be < xDim && >= 0
	stream << "CMP EAX, " << xDim << "\n";
	stream << "JGE >" << e2 << "\n";
	stream << "CMP EAX, 0\n";
	stream << "JL >" << e2 << "\n";
	stream << "JMP >" << l2 << "\n";
	stream << e2 << ":" << "\n";
	stream << "HLT\n";
	stream << l2 << ":" << "\n";

	stream << "IMUL EDX, " << xDim << "\n";		
	stream << "ADD EAX, EDX\n";
	stream << "IMUL EAX, 4\n";

	stream << "MOV EDX, " << offset << "\n";
	stream << "SUB EDX, EAX\n";

	//EDX must be <= offset && > 0
	stream << "CMP EDX, " << offset << "\n";
	stream << "JG >" << e1 << "\n";
	stream << "CMP EDX, 0\n";
	stream << "JLE >" << e1 << "\n";
	stream << "JMP >" << l1 << "\n";
	stream << e1 << ":" << "\n";
	stream << "HLT\n";
	stream << l1 << ":" << "\n";	

	stream << "NEG EDX\n";

	m_program << stream.str();
}

void Generator::AddWriteVarFromConsole (int const offset, string const type, int xDim)
{
	ostringstream stream;
	
	if (type == "int_array" || type == "bool_array") {
		HandleArray(offset);
		stream << "MOV [EBP + EDX], ECX\n\n";
	}
	else if (type == "int_double_array" || type == "bool_double_array") {		
		HandleDoubleArray(offset, xDim);
		stream << "MOV [EBP + EDX], ECX\n\n";
	}
	else {		
		stream << "MOV [EBP - " << offset << "], ECX\n\n";
	}
	m_program << stream.str();
}

void Generator::AddWriteVariable(int const offset, string const type, int xDim)
{
	ostringstream stream;

	if (type == "int_array" || type == "bool_array") {
		HandleArray(offset);
		stream << "POP EAX\n";
		stream << "MOV [EBP + EDX], EAX\n\n";
	}
	else if (type == "int_double_array" || type == "bool_double_array") {		
		HandleDoubleArray(offset, xDim);
		stream << "POP EAX\n";
		stream << "MOV [EBP + EDX], EAX\n\n";
	}
	else {
		stream << "POP EAX\n";
		stream << "MOV [EBP - " << offset << "], EAX\n\n";
	}

	m_program << stream.str();
}

void Generator::AddReadVariable(int const offset, string const type, int xDim) //read value from stack to EAX
{
	ostringstream stream;

	if (type == "int_array" || type == "bool_array") {
		HandleArray(offset);
		stream << "MOV EAX, [EBP + EDX]\n\n";
	}
	else if (type == "int_double_array" || type == "bool_double_array") {
		HandleDoubleArray(offset, xDim);
		stream << "MOV EAX, [EBP + EDX]\n\n";
	}
	else {
		stream << "MOV EAX, [EBP - " << offset << "]\n";
	}

	m_program << stream.str(); 
}

void Generator::BoolToConsole()
{
	if (!m_isPrintAdded) {
		AddPrintFunctionality();
		m_isPrintAdded = true;
	}

	ostringstream stream;	
	string l1 = GetLabelName();
	string l2 = GetLabelName();

	stream << "POP EDX\n";// take 'bool' value
	stream << "PUSH -11\n";
	stream << "CALL GetStdHandle\n";
	stream << "PUSH 0, ADDR RCKEEP\n";	

	stream << "CMP EDX, 1\n";
	stream << "JGE >" << l1 << "\n";	
	stream << "PUSH 5,'FALSE'\n";
	stream << "JMP >" << l2 << "\n";
	stream << l1 << ":\n";
	stream << "PUSH 4,'TRUE'\n";
	stream << l2 << ":\n";

	stream << "PUSH EAX\n";
	stream << "CALL WriteFile\n";
	
	m_program << stream.str();
}

void Generator::VarToConsole()
{
	if (!m_isPrintAdded) {
		AddPrintFunctionality();
		m_isPrintAdded = true;
	}

	ostringstream stream;	
	stream << "POP EAX\n";

	stream << "CALL CLEAR_BUFFER\n";
	stream << "CALL FILL_BUFFER\n";

	stream << "MOV EBX, EAX\n";

	stream << "PUSH CONSOLE_OUT_HANDLE_CODE\n";
	stream << "CALL GetStdHandle\n";

	stream << "PUSH 0\n";
	stream << "PUSH ADDR RCKEEP\n";

	stream << "MOV ECX, [CONST_10]\n";
	stream << "SUB ECX, EBX\n"; // The length of string depends on the number of cycles in FILL_BUFFER
	stream << "PUSH ECX\n";

	stream << "MOV ECX, ADDR BUFFER\n";
	stream << "ADD ECX, EBX\n"; // Address of string with offest, which removes leading zeros
	stream << "PUSH ECX\n";

	stream << "PUSH EAX\n";
	stream << "CALL WriteFile\n\n";

	m_program << stream.str();
}

void Generator::AddPrintFunctionality()
{
	ostringstream streamOfData;
	ostringstream streamOfProcedures;

	streamOfData << "CONSOLE_OUT_HANDLE_CODE EQU -11\n";

	if (!m_isInputOutputDataAdded) {
		streamOfData << "RCKEEP DD 0\n";
		streamOfData << "CONST_10 DD 10\n";
		streamOfData << "BUFFER DB 12 DUP ('0')\n"; // Extra space for new line and negative sign characters
		AddClearBuffer();
		m_isInputOutputDataAdded = true;
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
	streamOfProcedures << "RET\n\n";

	m_data << streamOfData.str();
	m_function << streamOfProcedures.str();
}

void Generator::AddClearBuffer()
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

	m_function << streamOfProcedures.str();
}

void Generator::WriteRelation(const string& rel)
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

	string trueLabel = GetLabelName();
	string endLabel = GetLabelName();
	command += " >" + trueLabel + "\n";

	stream << command;
	stream << "MOV EAX, 0\n";
	stream << ("JMP >" + endLabel + "\n");
	stream << (trueLabel) + ":\n";
	stream << "MOV EAX, 1\n";
	stream << (endLabel) + ":\n";
	stream << "PUSH EAX\n\n";
	
	m_program << stream.str();
}

string Generator::GetLabelName()
{
	string label = "@label" + std::to_string(static_cast<long long>(m_labelsCount));
	m_labelsCount++;
	return label;
}

void Generator::WriteAnd()
{
	ostringstream stream;
	
	string elseLabel = GetLabelName();
	string endLabel = GetLabelName();

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
	stream << (endLabel + ":\n\n");

	m_program << stream.str();
}

void Generator::WriteOr()
{
	ostringstream stream;
	
	string trueLabel = GetLabelName();
	string endLabel = GetLabelName();

	stream << "POP EBX\n";
	stream << "POP EAX\n";
	stream << "OR EAX, EBX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + trueLabel + "\n");
	stream << "PUSH 1\n";
	stream << ("JMP >" + endLabel + "\n");
	stream << (trueLabel + ":\n");
	stream << "PUSH 0\n";
	stream << (endLabel + ":\n\n");
	m_program << stream.str();
}

void Generator::WriteIfStart(string &falseLabel)
{
	ostringstream stream;

	falseLabel = GetLabelName();	
	stream << "POP EAX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + falseLabel + "\n\n");
	m_program << stream.str();
}

void Generator::WriteIfEnd(string falseLabel)
{
	ostringstream stream;		
	stream << (falseLabel + ":\n\n");
	m_program << stream.str();
}

string Generator::WriteIfMiddle(string &label)
{
	ostringstream stream;	

	string midLabel = GetLabelName();

	stream << "JMP " << midLabel << "\n";
	stream << (label + ":\n\n");
	m_program << stream.str();

	return midLabel;
}

string Generator::WriteWhileStartLabel(string &label)
{
	ostringstream stream;
	label = GetLabelName();	
	stream << (label + ":\n");	
	m_program << stream.str();

	string secondLabel = GetLabelName();	
	return secondLabel;
}

void Generator::WriteWhileStartPart(string label)
{
	ostringstream stream;	
	stream << "POP EAX\n";
	stream << "CMP EAX, 0\n";
	stream << ("JE >" + label + "\n\n");

	m_program << stream.str();
}

void Generator::WriteWhileEndPart(string label1, string label2)
{
	ostringstream stream;	
	stream << "JMP " << label1 << "\n";
	stream << (label2 + ":\n\n");
	m_program << stream.str();
}

void Generator::NagativVal()
{
	ostringstream stream;
	stream << "POP EAX\n";
	stream << "NEG EAX\n";
	stream << "PUSH EAX\n";
	m_program << stream.str();
}

void Generator::InverseExpression()
{
	ostringstream stream;	
	stream << "POP EAX\n";
	stream << "XOR EAX, 1\n";
	stream << "PUSH EAX\n";
	m_program << stream.str();
}

void Generator::WriteSomething()
{
	m_program << "ekrghioberpgbkerjbgkjlern\n";
}

void Generator::ConsoleToVar()
{
	if (!m_isReadAdded) {
		AddReadFunctionality();
		m_isReadAdded = true;
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
	stream << "MOV ECX, EAX\n\n";
	m_program << stream.str();
}

void Generator::AddReadFunctionality()
{
	ostringstream streamOfData;
	ostringstream streamOfProcedures;

	streamOfData << "CONSOLE_IN_HANDLE_CODE EQU -10\n";

	if (!m_isInputOutputDataAdded) {
		streamOfData << "RCKEEP DD 0\n";
		streamOfData << "CONST_10 DD 10\n";
		streamOfData << "BUFFER DB 12 DUP ('0')\n";
		AddClearBuffer();
		m_isInputOutputDataAdded = true;
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
	m_data << streamOfData.str();
	m_function << streamOfProcedures.str();
}


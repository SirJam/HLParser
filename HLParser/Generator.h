#pragma once

#include <sstream>
//#include "Variable.h"

using std::ofstream;
using std::ostringstream;

class Generator
{
public:
	Generator();

	void MakeASM(string fileName);
	void WriteProgramHead();	
	void WriteProgramEnd(int const varSpace);
	
	void AddToAsm();	
	void SubToAsm();
	void MulToAsm();
	void DivToAsm();
	void ModuloToAsm();

	void NagativVal();
	void InverseExpression();

	void AllocateSpace(int const space);

	void WriteIntVar(int const offset, string const type, int x = 0);
	void WriteIntConst(int num);
		
	void VarToConsole();	
	void BoolToConsole();
	void ConsoleToVar();

	void WriteAssignment(int const offset, string const type, int x = 0);

	void WriteRelation(const string& relOperator);
	void WriteAnd();
	void WriteOr();

	void WriteIfStart(string &falseLabel);
	void WriteIfEnd(string falseLabel);
	string WriteIfMiddle(string &label);

	string WriteWhileStartLabel(string &label);
	void WriteWhileStartPart(string label);
	void WriteWhileEndPart(string label1, string label2);

	void AddReadVariable(int const offset, string const varType, int xDim = 0);
	void AddWriteVariable(int const offset, string const type, int xDim = 0);
	void AddWriteVarFromConsole(int const offset, string const type, int xDim = 0);

	void WriteSomething();
		
private:
	ostringstream m_program;
	ostringstream m_data;
	ostringstream m_function;
	
	int m_labelsCount;
	bool m_isPrintAdded, m_isInputOutputDataAdded, m_isReadAdded;

	void AddPrintFunctionality();
	void AddReadFunctionality();
	void AddClearBuffer();			
	string GetLabelName();	
	void HandleArray(int offset);
	void HandleDoubleArray(int offset, int xDim);
};

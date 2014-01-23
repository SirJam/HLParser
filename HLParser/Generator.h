#pragma once

#include <sstream>
//#include "Variable.h"

using std::ofstream;
using std::ostringstream;

class Generator
{
public:
	Generator();

	void createASMFile(string fileName);
	void createProgramStart();	
	void createProgramEnd(int const varSpace);
	
	void createAddOperation();	
	void createSubstractOperation();
	void createMultiplyOperation();
	void createDivideOperation();

	void createVariableSpace(int const space);

	void createIntVariable(int const offset, string const type, int x = 0);
	void createIntConstant(int num);
		
	void createPrintInteger();	

	void createAssignmentOperation(int const offset, string const type, int x = 0);

	void createRelExpression(const string& relOperator);
	void applyAndExpression();
	void applyOrExpression();

	void createIfExpressionStartPart(string &falseLabel);
	void createIfExpressionEndPart(string falseLabel);

	void addReadVariable(int const offset, string const varType, int xDim = 0);
	void addWriteVariable(int const offset, string const type, int xDim = 0);

	void writeSomething();
		
private:
	ostringstream programStream;
	ostringstream dataStream;
	ostringstream procedureStream;
	
	int labelsCount;
	bool isPrintAdded, isInputOutputDataAdded, isReadAdded, isMainStringBufferAdded;

	void addPrintFunctionality();
	void addClearBuffer();	
	string getLabelName();	
};

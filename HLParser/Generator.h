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

	void createIntVariable(int const offset, string const type);
	void createIntConstant(int num);

	void createAssignmentOperation(int const offset, bool const isInArray, int const typeSize);

	void addReadVariable(int const offset, string const varType);
	void addWriteVariable(int const offset, bool const isInArray, int const typeSize);

		
private:
	ostringstream programStream;
	ostringstream dataStream;
	ostringstream procedureStream;
	string cleanfileName;	
};

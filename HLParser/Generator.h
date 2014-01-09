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
	void createProgramEnd();

	
	void createAddOperation();	
	void createSubstractOperation();
	void createMultiplyOperation();
	void createDivideOperation();

	void createVariableSpace(int const space);

	void createIntVariable(int const offset, bool const isInArray, int const typeSize);
	void createIntConstant(int num);

	void addReadVariable(int const offset, bool const isInArray, int const typeSize);

	//void createIntConstant(int num);

	//void addClearBuffer();

	//void createPrintInteger(bool const addNewLine);
		
private:
	ostringstream programStream;
	ostringstream dataStream;
	ostringstream procedureStream;
	string cleanfileName;	
};

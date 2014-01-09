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

	void createVariable(string name, int const value, bool const isArray, int const arrSize);

	void createAddOperation();	
	void createSubstractOperation();
	void createMultiplyOperation();
	void createDivideOperation();

	void addClearBuffer();

	void createPrintInteger(bool const addNewLine);
		
private:
	ostringstream programStream;
	ostringstream dataStream;
	ostringstream procedureStream;
	string cleanfileName;	
};

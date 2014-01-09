/*#pragma once

#include <sstream>
//#include "Variable.h"

using std::ofstream;
using std::ostringstream;

class Generator
{
public:
	Generator();

	void writeToFile(string fileName);
	void writeToConsole();

	void createExecutable();
	string getExecutableName();

	void createObjectFile();

	void createProgramStart();
	void createProgramEnd();

	void createAddOperation();	
		
	void createIntVariable(int const offset, bool const isInArray, int const typeSize);
	

	void createVariableSpace(int const variableSpace);
	void removeVariableSpace(int const variableSpace);	

	void createAssignmentOperation(int const offset, bool const isInArray, int const typeSize);
	

	//void createAssignmentWithAddOperation(int const offset, bool const isInArray, int const typeSize);
	//void createAssignmentWithSubstractOperation(int const offset, bool const isInArray, int const typeSize);

	//void negateResult();
	void createExpressionInversion();
	void createRelExpression(const string& relOperator);
	
	//void createIfExpressionStartPart(string &falseLabel);
	//void createIfExpressionEndPart(string falseLabel);

	/* Function calls */
	//void createPrintString(bool const addNewLine);
	//void createPrintInteger(bool const addNewLine);
	//void createPrintBool(bool const addNewLine);

	//void createReadIntToVariable(int const offset, bool const isInArray, int const typeSize);
	

	//void applyAndExpression();
	//void applyOrExpression();

	//void createCthulhuFunction();

	//string getFreeLabelName();
/*private:
	ostringstream programStream;
	ostringstream dataStream;
	ostringstream procedureStream;
	string cleanfileName;

	bool isPrintAdded, isInputOutputDataAdded, isReadAdded, isMainStringBufferAdded;
	//void addPrintFunctionality();
	//void addReadFunctionality();

	//void addReadVariable(int const offset, bool const isInArray, int const typeSize);
	//void addWriteVariable(int const offset, bool const isInArray, int const typeSize);
	//void addClearBuffer();
	//void addMainStringBuffers();

	

	//unsigned int labelsCreated;
};*/

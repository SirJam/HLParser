#include "StdAfx.h"
#include "Token.h"
#include "VariableManager.h"
#include "FunctionManager.h"
#include "ErrorHandler.h"

VariableManager::VariableManager(vector<Token> &tokens)
:lastVariableCount(0), 
maxStringCount(0), 
currentStringCount(0)
{
	this->parserTokens = &tokens;
}

/*void VariableManager::setFunctionManager(FunctionManager &functionManager)
{
	this->functionManager = &functionManager;
}

Variable *VariableManager::findVariable(Token const *variableToken)
{
	string const variableName = variableToken->value;
	Variable *variable = NULL;

	for(vector<Variable>::reverse_iterator rit = variables.rbegin(); rit != variables.rend(); rit++) {
		if ((*rit).name == variableName) {
			return &(*rit);
		}
	}

	if (variable == NULL) {
		ErrorHandler::failWithErrorCode(302, variableName, variableToken->lineNumber);
	}

	return variable;
}

int VariableManager::findVariablePosition(Token const *variableToken)
{
	string const variableName = variableToken->value;

	bool variableFound = false;
	int variableNumber = variables.size();
	for(vector<Variable>::reverse_iterator rit = variables.rbegin(); rit != variables.rend(); rit++) {
		if ((*rit).name == variableName) {
			variableFound = true;
			break;
		} else {
			variableNumber--;
		}
	}
	if (!variableFound) {
		ErrorHandler::failWithErrorCode(302, variableName, variableToken->lineNumber);
	}
	return variableNumber;
}

int VariableManager::findStringPosition(Token const *variableToken)
{
	string const variableName = variableToken->value;
	bool variableFound = false;
	int stringNumber = currentStringCount - 1;

	for(vector<Variable>::reverse_iterator rit = variables.rbegin(); rit != variables.rend(); rit++) {
		Variable *variable = &(*rit);
		if (variable->name == variableName) {
			variableFound = true;
			stringNumber -= (variable->count - 1); // in case of array, we should be at the beginning of it
			break;
		} else if ((*rit).type == TYPE_STRING) {
			stringNumber--;
		}
	}
	if (!variableFound) {
		ErrorHandler::failWithErrorCode(302, variableName, variableToken->lineNumber);
	}
	return stringNumber;
}

int VariableManager::calculateVariableOffset(int const variableNumber)
{
	int offset = 0;
	for (int i = 0; i < variableNumber; i++) {
		Variable *variable = &variables[i];
		offset += sizeOf(variable->type) * variable->count;
	}
	return offset;
}

void VariableManager::generateVariableInExpression()
{
	Token *varToken = &parserTokens->end()[-1];
			
	Token *idToken = typeChecker->getTokenId(*varToken);
	bool isInArray = typeChecker->isInArray;
	
	Variable *variable = findVariable(idToken);
	typeChecker->checkVariableType(*variable, isInArray, *idToken);

	TYPE type = variable->type;

	switch (type) {
		case TYPE_INT: {
			int variableNumber = findVariablePosition(idToken);
			int offset = calculateVariableOffset(variableNumber);
			generator->createIntVariable(offset, isInArray, sizeOf(variable->type));
			break;
		}
		case TYPE_STRING: {
			bool isAfterComma = (parserTokens->end()[-2].symbol.name == ","); // It means the string is in a function call
			bool isAfterBracket = (parserTokens->end()[-2].symbol.name == "(");
			bool isAfterAssign = (parserTokens->end()[-2].symbol.name == "=");

			int stringNumber = findStringPosition(idToken);
			generator->createStringVariable(stringNumber, isInArray, (isAfterComma || isAfterBracket || isAfterAssign), isAfterBracket);
			break;
		}
		case TYPE_BOOL: {
			int variableNumber = findVariablePosition(idToken);
			int offset = calculateVariableOffset(variableNumber);
			generator->createBoolVariable(offset, isInArray, sizeOf(variable->type));
			break;		
		}
	}
}*/
/*
void VariableManager::generateAssignment()
{
	Token *varToken = &parserTokens->end()[-3];

	Token *idToken = typeChecker->getTokenId(*varToken);
	bool isInArray = typeChecker->isInArray;

	Token *operationToken = &parserTokens->end()[-2];
	string operation = operationToken->value;

	if (operation == "=" || operation == "+=" || operation == "-=") {
		Variable *variable = findVariable(idToken);
		typeChecker->checkVariableType(*variable, isInArray, *idToken);

		TYPE variableType, expressionType;
		variableType = variable->type;

		// Checking match between variable and expression types
		Token *expressionToken = typeChecker->getTypeToken(parserTokens->back());
		if (expressionToken->symbol.name == "var") {
			Token *tokenId = typeChecker->getTokenId(*expressionToken);
			Variable *variable = findVariable(tokenId);
			expressionType = variable->type;
		} else if (expressionToken->symbol.name == "constant") {
			expressionType = typeChecker->checkConstant(*expressionToken);
		} else if (expressionToken->symbol.name == "call") {
			string functionName = expressionToken->formingTokens.end()[-1].value;
			expressionType = functionManager->getFunctionType(functionName);
		}
		typeChecker->compareAssignmentTypes(variableType, expressionType, idToken->lineNumber);

		int variableNumber = findVariablePosition(idToken);
		int offset = calculateVariableOffset(variableNumber);

		if (operation == "=") {
			switch (variableType) {
				case TYPE_INT:
				case TYPE_BOOL:
					generator->createAssignmentOperation(offset, isInArray, sizeOf(variable->type));
					break;
				case TYPE_STRING: {
					int stringNumber = findStringPosition(idToken);
					generator->createStringAssignmentOperation(stringNumber, isInArray);
					break;
				}
			}
		} 
		else if (operation == "+=") {
			switch (variableType) {
				case TYPE_INT:
					generator->createAssignmentWithAddOperation(offset, isInArray, sizeOf(variable->type));
					break;
				default:
					ErrorHandler::failWithErrorCode(504, operation, typeAsString(variableType), operationToken->lineNumber);
					break;
			}
		} 
		else if (operation == "-=") {
			switch (variableType) {
				case TYPE_INT:
					generator->createAssignmentWithSubstractOperation(offset, isInArray, sizeOf(variable->type));
					break;
				default:
					ErrorHandler::failWithErrorCode(504, operation, typeAsString(variableType), operationToken->lineNumber);
					break;
			}
		}
	}
}

void VariableManager::generateVariableSpace()
{
	for (int i = 1; i <= lastVariableCount; i++) {
		Variable *variable = &variables[variables.size() - i];
		int variableSpace = sizeOf(variable->type) * variable->count;

		generator->createVariableSpace(variableSpace);
	}
		
	if (lastVariableCount) {
		variableCount.push_back(lastVariableCount);
		lastVariableCount = 0;
	}
}

void VariableManager::generateRemoveVariableSpace()
{
	Token *localDeclarations = &parserTokens->end()[-3];
	if (variableCount.size() && localDeclarations->formingTokens.size()) {
		int count = variableCount.back();
		for (int i = 0; i < count; i++) {
			Variable *variable = &variables[variables.size() - 1];
			int variableSpace = sizeOf(variable->type) * variable->count;

			//generator->removeVariableSpace(variableSpace);

			if (variable->type == TYPE_STRING) {
				currentStringCount -= variable->count;
			}

			variables.pop_back();
		} 
		variableCount.pop_back();
	}
}

void VariableManager::generateStringSpace()
{
	if (maxStringCount) {
		//generator->createStringSpace(maxStringCount);
	}
}

void VariableManager::addVariable()
{
	Token *typeToken = &parserTokens->end()[-3];
	string type = typeToken->formingTokens.front().value;

	Token *varDeclList = &parserTokens->end()[-2];
	while (varDeclList) {
		Variable variable;
		Token *idToken = NULL;
		int count = 1;

		Token *varDeclId = &varDeclList->formingTokens[0];
		if (varDeclId->formingTokens.size() == 1) {
			idToken = &varDeclId->formingTokens.back();
		} else {
			idToken = &varDeclId->formingTokens.end()[-1];
			count = stoi(varDeclId->formingTokens.end()[-3].value);
		}

		if (variables.size()) { // Checking variable redecleration in same scope
			for (int i = variables.size() - 1; i >= (int)variables.size() - lastVariableCount; i--) {
				if (i < 0) {
					break;
				}
				Variable *declaredVariable = &variables[i];
				if (declaredVariable->name == idToken->value) {
					ErrorHandler::failWithErrorCode(304, idToken->value, idToken->lineNumber);
				}
			}
		}

		variable.count = count;
		variable.name = idToken->value;
		variable.type = stringAsType(type);
		if (variable.type == TYPE_STRING) {
			currentStringCount += count;
			if (maxStringCount < currentStringCount) {
				maxStringCount = currentStringCount;
			}
		}
		if (variable.type == TYPE_VOID) {
			ErrorHandler::failWithErrorCode(506, typeAsString(variable.type), idToken->lineNumber);
		}

		variables.push_back(variable);
		lastVariableCount++;

		if (isVariableNameCommentaryEnabled) {
			addVariableCommentary(variable, *idToken);
		}

		if (varDeclList->formingTokens.size() == 3) {
			varDeclList = &varDeclList->formingTokens.back();
		} else {
			varDeclList = NULL;
		}
	}
}

void VariableManager::addVariableCommentary(Variable const& variable, Token const& idToken)
{
	if (variable.name.length() == 1 || variable.name == "count" || variable.name == "ololo") {
		int lineNumber = idToken.lineNumber % 10;
		ostringstream stream;

		if (lineNumber == 2) { 
			stream << "[By the way, " << variable.name << " is the most unimaginative name I have ever seen.]";
		} else if (lineNumber == 3) {
			stream << "[Name " << variable.name << " is pretty terrible. You can do better.]";
		} else if (lineNumber == 4) {
			stream << "[Really? " << variable.name << "? Wow. I mean. Wow.]";
		}

		if (stream.str().length()) {
			generator->createStringConstant(stream.str(), false, false);
			generator->createPrintString(true);
		}
	}
}
*/
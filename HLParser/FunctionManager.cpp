#include "StdAfx.h"
#include "FunctionManager.h"
#include "ErrorHandler.h"

FunctionManager::FunctionManager(vector<Token> &tokens, VariableManager &variableManager)
{
	this->parserTokens = &tokens;
	this->variableManager = &variableManager;

	numberOfArguments["print"] = 1;       functionTypes["print"] = TYPE_VOID;
	numberOfArguments["pringles"] = 1;    functionTypes["pringles"] = TYPE_VOID;
	numberOfArguments["read"] = 1;        functionTypes["read"] = TYPE_VOID;
	numberOfArguments["cthulhu"] = 0;     functionTypes["cthulhu"] = TYPE_VOID;
	numberOfArguments["substring"] = 2;   functionTypes["substring"] = TYPE_INT;
}

/*TYPE FunctionManager::getFunctionType(string const& functionName)
{
	return functionTypes.find(functionName)->second;
}

void FunctionManager::checkMainFunction()
{
	Token *tokenId = &parserTokens->end()[-5];

	// Checking main function name
	string functionName = tokenId->value;
	if (functionName != "main" && functionName != "noclip") {
		ErrorHandler::failWithErrorCode(404, functionName, tokenId->lineNumber);
	}

	// Checking main function arguments
	Token *tokenParams = &parserTokens->end()[-3];
	if (tokenParams->formingTokens.size()) {
		ErrorHandler::failWithErrorCode(405, functionName, tokenId->lineNumber);
	}

	// Checking main function type
	Token *tokenTypeSpecifier = &parserTokens->end()[-6];
	Token *tokenType = &tokenTypeSpecifier->formingTokens.back();
	typeChecker->checkFunctionType(*tokenType, TYPE_INT);
}

void FunctionManager::generateFunctionCall()
{
	Token *functionToken = &parserTokens->end()[-4];
	string functionName = functionToken->value;

	int argumentCount = findNumberOfArguments();

	map<string, int>::iterator it = numberOfArguments.find(functionName);
	if (it == numberOfArguments.end()) { // Function not found
		ErrorHandler::failWithErrorCode(402, functionName, functionToken->lineNumber);
	} else if (it->second != argumentCount) { // Number of arguments doesn't match
		ErrorHandler::failWithErrorCode(403, functionName, std::to_string(static_cast<long long>(argumentCount)), functionToken->lineNumber);
	}

	TYPE firstArgumentType, secondArgumentType;
	if (argumentCount) {
		firstArgumentType = findArgumentType(1, argumentCount);
	}
	if (argumentCount >= 2) {
		secondArgumentType = findArgumentType(2, argumentCount);
	}

	bool hasNegativeSign = (parserTokens->end()[-5].value == "-");
	TYPE functionType = getFunctionType(functionName);
	if (hasNegativeSign && functionType != TYPE_INT) {
		ErrorHandler::failWithErrorCode(505, typeAsString(functionType), functionToken->lineNumber);
	}

	if (functionName == "print") {
		switch (firstArgumentType) {
			case TYPE_INT:
				generator->createPrintInteger(false);
				break;
			case TYPE_STRING:
				generator->createPrintString(false);
				break;
			case TYPE_BOOL:
				generator->createPrintBool(false);
				break;
			default:
				ErrorHandler::failWithErrorCode(401, functionName, typeAsString(firstArgumentType), functionToken->lineNumber);
				break;
		}
	}
	else if (functionName == "pringles") {
		switch (firstArgumentType) {
			case TYPE_INT:
				generator->createPrintInteger(true);
				break;
			case TYPE_STRING:
				generator->createPrintString(true);
				break;
			case TYPE_BOOL:
				generator->createPrintBool(true);
				break;
			default:
				ErrorHandler::failWithErrorCode(401, functionName, typeAsString(firstArgumentType), functionToken->lineNumber);
				break;
		}
	}
	else if (functionName == "read") {
		Token *varToken = getVariableToken(*functionToken);
		Variable *variable = variableManager->findVariable(varToken);

		bool isInArray = (variable->count > 1);
		int variableSize = sizeOf(variable->type);

		switch (firstArgumentType) {
			case TYPE_INT: {
				int variableNumber = variableManager->findVariablePosition(varToken);
				int offset = variableManager->calculateVariableOffset(variableNumber);
				generator->createReadIntToVariable(offset, isInArray, variableSize);
				break;
			}
			case TYPE_STRING: {
				int stringPosition = variableManager->findStringPosition(varToken);
				generator->createReadStringToVariable(stringPosition, isInArray);
				break;
			}
			default:
				ErrorHandler::failWithErrorCode(401, functionName, typeAsString(firstArgumentType), functionToken->lineNumber);
				break;
		}
	}
	else if (functionName == "substring") {
		if (firstArgumentType != TYPE_STRING) {
			ErrorHandler::failWithErrorCode(401, functionName, typeAsString(firstArgumentType), functionToken->lineNumber);
		}
		else if (secondArgumentType != TYPE_STRING) {
			ErrorHandler::failWithErrorCode(401, functionName, typeAsString(secondArgumentType), functionToken->lineNumber);
		}
		else {
			generator->createSubstring();
		}
	}
	else if (functionName == "cthulhu" && areSecretFunctionsEnabled) {
		generator->createCthulhuFunction();
	}
	else {
		ErrorHandler::failWithErrorCode(402, functionName, functionToken->lineNumber);
	}
}

int FunctionManager::findNumberOfArguments()
{
	int arguments = 0;

	Token *argsToken = &parserTokens->end()[-2];
	if (argsToken->formingTokens.size()) {
		Token *argListToken = &argsToken->formingTokens.back();
		while (argListToken->formingTokens.front().symbol.name == "expression") {
			arguments++;
			argListToken = &argListToken->formingTokens.back();
		}
	}

	return arguments;
}

TYPE FunctionManager::findArgumentType(int const argumentNumber, int const totalArgumentNumber)
{
	TYPE type;
	int argumentCount = totalArgumentNumber;

	Token *argsToken = &parserTokens->end()[-2];
	Token *argList = &argsToken->formingTokens[0];

	while (argumentCount > 2) {
		argumentCount--;
		argList = &argList->formingTokens.back();
	}

	if (hasFormingTokenWithName(argList, "relop") // If function call has comparing or logic operators
	 || hasFormingTokenWithName(argList, "|")
	 || hasFormingTokenWithName(argList, "&")
	 || hasFormingTokenWithName(argList, "!")
	   ) {
		return TYPE_BOOL;
	}

	Token *varToken;
	if (argumentNumber % 2 == 0) {
		varToken = &argList->formingTokens.end()[-3];
	} else {
		varToken = &argList->formingTokens.back();
	}

	if (varToken->formingTokens.size()) {
		while (varToken->symbol.name != "var" && varToken->symbol.name != "call" && varToken->symbol.name != "constant") {
			Token *nextToken = &varToken->formingTokens[0];
			if (nextToken->symbol.name == "(" || nextToken->symbol.name == ")") {
				nextToken = &varToken->formingTokens[1];
			}
			varToken = nextToken;
		}
	}

	Token *typeToken = &varToken->formingTokens.back();
	if (typeToken->symbol.name == "num") {
		type = TYPE_INT;
	} 
	else if (varToken->symbol.name == "call") {
		type = getFunctionType(typeToken->value);
	}
	else if (typeToken->symbol.name == "stringconst") {
		type = TYPE_STRING;
	} else if (typeToken->symbol.name == "true" || typeToken->symbol.name == "false") {
		type = TYPE_BOOL;		
	} else if (typeToken->symbol.name == "id") {
		Variable *variable = variableManager->findVariable(typeToken);
		type = variable->type;
	}

	return type;
}

Token *FunctionManager::getVariableToken(Token const& functionToken)
{
	Token *varToken = &parserTokens->end()[-2];

	if (varToken->formingTokens.size()) {
		while (varToken->symbol.name != "var") {
			if (!varToken->formingTokens.size() || varToken->formingTokens.size() > 1) {
				ErrorHandler::failWithErrorCode(406, functionToken.value, functionToken.lineNumber);
			}
			varToken = &varToken->formingTokens[0];
		}
	}

	return &varToken->formingTokens.back();
}

bool FunctionManager::hasFormingTokenWithName(Token *token, string name)
{
	for (vector<Token>::iterator it = token->formingTokens.begin(); it != token->formingTokens.end(); it++) {
		if (it->symbol.name == name || hasFormingTokenWithName(&(*it), name)) {
			return true;
		}
	}
	return false;
}
*/
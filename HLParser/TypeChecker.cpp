#include "StdAfx.h"
#include "Variable.h"
#include "TypeChecker.h"
#include "FunctionManager.h"
#include "VariableManager.h"
#include "ErrorHandler.h"

TypeChecker::TypeChecker(vector<Token> &tokens)
{
	this->parserTokens = &tokens;
}

void TypeChecker::setFunctionManager(FunctionManager &functionManager)
{
	this->functionManager = &functionManager;
}

void TypeChecker::setVariableManager(VariableManager &variableManager)
{
	this->variableManager = &variableManager;
}

void TypeChecker::resetLastValues()
{
	lastString = "";
	lastInt = 0;
	lastBool = false;
	isInArray = false;
}

void TypeChecker::checkFunctionType(Token const& tokenType, TYPE expectedType)
{
	TYPE currentType = stringAsType(tokenType.value);
	if (currentType != expectedType) {
		ErrorHandler::failWithErrorCode(502, typeAsString(expectedType), tokenType.value, tokenType.lineNumber); 
	}
}

void TypeChecker::checkVariableType(Variable const& variable, bool isInArray, Token const& idToken)
{
	if (isInArray && variable.count == 1) {
		ErrorHandler::failWithErrorCode(301, idToken.value, idToken.lineNumber);
	} else if (!isInArray && variable.count > 1) {
		ErrorHandler::failWithErrorCode(303, idToken.value, idToken.lineNumber);
	}
}

TYPE TypeChecker::checkConstant(Token const& tokenConstant)
{
	resetLastValues();
	TYPE type;
	Token const *typeToken = &tokenConstant.formingTokens.back();

	if (typeToken->symbol.name == "num") {
		int value = stoi(typeToken->value);
		type = TYPE_INT;
		lastInt = value;
	} else if (typeToken->symbol.name == "stringconst") {
		type = TYPE_STRING;
		lastString = typeToken->value;
	} else if (typeToken->symbol.name == "true" || typeToken->symbol.name == "false") {
		type = TYPE_BOOL;
		lastBool = (typeToken->symbol.name == "true") ? true : false;	
	}

	if (parserTokens->end()[-2].value == "[" && type != TYPE_INT) { // If a constant in an array is not an int
		ErrorHandler::failWithErrorCode(501, typeAsString(TYPE_INT), typeAsString(type), typeToken->lineNumber); 
	}

	return type;
}

/* The 'var' token is passed here, so we get it's id token and find out whether it's an array or not */
Token *TypeChecker::getTokenId(Token &varToken)
{
	resetLastValues();
	Token *idToken = NULL;

	if (varToken.formingTokens.size() == 1) {
		idToken = &varToken.formingTokens.back();
	} else if (varToken.formingTokens.size() == 4) {
		isInArray = true;
		idToken = &varToken.formingTokens.end()[-1];
	}
	return idToken;
}

/* Goes through forming tokens until 'var', 'constant' or 'call' token */
Token *TypeChecker::getTypeToken(Token &token)
{
	Token *tokenFactor = &token;

	while (tokenFactor->symbol.name != "var" && tokenFactor->symbol.name != "constant" && tokenFactor->symbol.name != "call") {
		if (tokenFactor->formingTokens.size() == 3 && tokenFactor->symbol.name == "factor") {
			tokenFactor = &tokenFactor->formingTokens[1];
		} else {
			tokenFactor = &tokenFactor->formingTokens[0];
		}
	}

	return tokenFactor;
}

void TypeChecker::compareAssignmentTypes(TYPE const variableType, TYPE const expressionType, int const lineNumber)
{
	// Because logical expressions with int are converted to bool type
	if (variableType == TYPE_BOOL &&
		(expressionType == TYPE_BOOL || expressionType == TYPE_INT))
		return;

	compareTwoTypes(variableType, expressionType, lineNumber);
}

void TypeChecker::compareTwoTypes(TYPE const type1, TYPE const type2, int const lineNumber)
{
	if (type1 != type2) {
		ErrorHandler::failWithErrorCode(503, typeAsString(type1), typeAsString(type2), lineNumber);
	}
}

/* Checks the type of token before applying negative sign '-' on it */
void TypeChecker::checkNegationType()
{
	Token *typeToken = getTypeToken(parserTokens->back());
	TYPE type;

	if (typeToken->symbol.name == "var") {
		Token *tokenId = getTokenId(*typeToken);
		Variable *variable = variableManager->findVariable(tokenId);
		type = variable->type;
	}
	else if (typeToken->symbol.name == "call") {
		string functionName = typeToken->formingTokens.end()[-1].value;
		type = functionManager->getFunctionType(functionName);
	}
	else {
		type = checkConstant(*typeToken);
	}

	switch (type) {
		case TYPE_INT:
			return;
			break;
		default:
			ErrorHandler::failWithErrorCode(505, typeAsString(type), typeToken->lineNumber);
			break;
	}
}

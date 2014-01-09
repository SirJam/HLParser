#include "stdafx.h"
#include "VariablesTable.h"
#include "SizeManager.h"


VariablesTable::VariablesTable()
{
}

VariablesTable::~VariablesTable()
{
}

bool VariablesTable::TryToRegisterVariable(vector<Token> stack)
{
	Token typeToken = stack.at(stack.size() - 3);
	string type = typeToken.formingTokens.front().formingTokens.front().value;


	/*Token *varDeclList = &stack.at(stack.size() - 2);
	while (varDeclList) {
		Variable variable;
		Token *idToken = NULL;
		int count = 1;

		Token *varDeclId = &varDeclList->formingTokens[0];
		if (varDeclId->formingTokens.size() == 0) {
			idToken = varDeclId;
		}
		else
		{
			idToken = &varDeclId->formingTokens.end()[-1];
			count = stoi(varDeclId->formingTokens.end()[-3].value);*/

	Token *identifiersDefinition = &stack.at(stack.size() - 2);
	while (identifiersDefinition) {
		Token *idToken = NULL;
		Token *identifierDefinition;
		Variable variable;
		int count = 1;

		if (identifiersDefinition->formingTokens.size())
		{
			identifierDefinition = &identifiersDefinition->formingTokens[0];
		}
		else
		{
			identifierDefinition = identifiersDefinition;
		}
		if (identifierDefinition->formingTokens.size() == 0) {
			idToken = identifierDefinition;
		}
		else
		{
			idToken = &identifierDefinition->formingTokens.at(identifierDefinition->formingTokens.size() - 1);
	//		count = stoi(varDeclId->formingTokens.end()[-3].value);

		}

		/*if (m_variablesTable.size()) { // Checking variable redecleration in same scope
			for (int i = m_variablesTable.size() - 1; i >= (int) m_variablesTable.size() - lastVariableCount; i--) {
				if (i < 0) {
					break;
				}
				Variable *declaredVariable = &m_variablesTable[i];
				if (declaredVariable->m_name == idToken->value) {
					exit(EXIT_FAILURE);
					//ErrorHandler::failWithErrorCode(304, idToken->value, idToken->lineNumber);
				}
			}
		}*/

		//variable.count = count;
		variable.m_name = idToken->value; 
		variable.m_type = type;
		//variable.m_type = stringAsType(type);

		variable.m_size = SizeManager::SizeOfType(variable.m_type);

		m_variablesTable.push_back(variable);
		//lastVariableCount++;


		/*if (varDeclList->formingTokens.size() == 3) {
			varDeclList = &varDeclList->formingTokens.back();
		}
		else {
			varDeclList = NULL;*/

		if (identifiersDefinition->formingTokens.size() == 3) {
			identifiersDefinition = &identifiersDefinition->formingTokens.back();
		}
		else {
			identifiersDefinition = NULL;
		}
	}
	
	return true;
}
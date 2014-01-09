#include "stdafx.h"
#include "VariablesTable.h"


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

	Token *identifiersDefinition = &stack.at(stack.size() - 2);
	while (identifiersDefinition) {
		Token *idToken = NULL;
		Token *varDeclId;
		Variable variable;
		int count = 1;

		if (identifiersDefinition->formingTokens.size())
		{
			varDeclId = &identifiersDefinition->formingTokens[0];
		}
		else
		{
			varDeclId = identifiersDefinition;
		}
		if (varDeclId->formingTokens.size() == 0) {
			idToken = varDeclId;
		}
		else
		{
			idToken = &varDeclId->formingTokens.at(varDeclId->formingTokens.size() - 1);
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

		m_variablesTable.push_back(variable);
		//lastVariableCount++;

		if (identifiersDefinition->formingTokens.size() == 3) {
			identifiersDefinition = &identifiersDefinition->formingTokens.back();
		}
		else {
			identifiersDefinition = NULL;
		}
	}
	
	return true;
}
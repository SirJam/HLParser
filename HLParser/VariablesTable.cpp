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

	Token *varDeclList = &stack.at(stack.size() - 2);
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
		//variable.m_type = stringAsType(type);

		m_variablesTable.push_back(variable);
		//lastVariableCount++;

		if (varDeclList->formingTokens.size() == 3) {
			varDeclList = &varDeclList->formingTokens.back();
		}
		else {
			varDeclList = NULL;
		}
	}
	
	return true;
}
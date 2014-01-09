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
	string type;
	int factor = 1;

	if (stack.at(stack.size() - 3).formingTokens.back().symbol.name == "primitive_type")
	{
		typeToken = stack.at(stack.size() - 3);
		type = typeToken.formingTokens.front().formingTokens.front().value;
	}
	else
	{
		typeToken = stack.at(stack.size() - 3).formingTokens.back().formingTokens.back();
		type = typeToken.formingTokens.front().value;
		factor = stoi(stack.at(stack.size() - 3).formingTokens.back().formingTokens[1].formingTokens.back().formingTokens.back().value);
	}

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
		variable.m_size = SizeManager::SizeOfType(variable.m_type) * factor;

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
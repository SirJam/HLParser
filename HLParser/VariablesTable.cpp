#include "stdafx.h"
#include "VariablesTable.h"
#include "SizeManager.h"
#include "TypeChecker.h"
#include "ErrorHandler.h"
#include "RuleName.h"
#include "ReservedWords.h"

VariablesTable::VariablesTable()
{
	m_variablesTable = new vector<Variable>;
}

VariablesTable::~VariablesTable()
{
}

bool VariablesTable::TryToRegisterVariable(vector<Token> stack)
{
	Token typeToken = stack.at(stack.size() - 3);
	string type;
	int factor = 1;

	if (stack.at(stack.size() - 3).formingTokens.back().symbol.name == RuleName::PRIMITIVE_TYPE())
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

	if (!TypeChecker::IsAllowedType(type))
	{
		ErrorHandler::FailedWithTypeError(type, typeToken.lineNumber);
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

		if (identifierDefinition->formingTokens.size() == 0) 
		{
			idToken = identifierDefinition;
		}
		else
		{
			idToken = &identifierDefinition->formingTokens.at(identifierDefinition->formingTokens.size() - 1);
		}

		if (ReservedWords::IsReservedWord(idToken->value))
		{
			ErrorHandler::FailedWithReservedWord(idToken->value, idToken->lineNumber);
		}

		if (m_variablesTable->size()) 
		{
			for (Variable var : *m_variablesTable)
			{
				if (var.m_name == idToken->value)
				{
					ErrorHandler::FailedWithRedefinition(idToken->value, idToken->lineNumber);
				}
			}
		}

		variable.m_name = idToken->value; 
		variable.m_type = type;
		variable.m_size = SizeManager::SizeOfType(variable.m_type) * factor;

		m_variablesTable->push_back(variable);

		if (identifiersDefinition->formingTokens.size() == 3) 
		{
			identifiersDefinition = &identifiersDefinition->formingTokens.back();
		}
		else 
		{
			identifiersDefinition = NULL;
		}
	}
	
	return true;
}

void VariablesTable::CheckExistingOfVariable(vector<Token> stack)
{
	Token idToken = stack.at(stack.size() - 3);
	//Token 
}
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

Token *VariablesTable::GetLastVariable(Token *token)
{
	if (token->formingTokens.size())
	{
		token = GetLastVariable(&token->formingTokens[0]);	
	}
	else
	{
		return token;
	}
}

void VariablesTable::GetBodyWithoutTail(Token & token)
{
	if (token.formingTokens.size() == 3)
	{
		GetBodyWithoutTail(token.formingTokens[0]);
	}
	else
	{

	}
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

		identifierDefinition = &identifiersDefinition->formingTokens.back();
		if (identifierDefinition->formingTokens.size() == 1) 
		{
			idToken = &identifierDefinition->formingTokens.front();
		}
		else 
		{
			idToken = identifierDefinition;
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
			identifiersDefinition = &identifiersDefinition->formingTokens.front();
		}
		else 
		{
			identifiersDefinition = NULL;
		}
	}
	
	return true;
}

void VariablesTable::CheckExistingOfVariable(vector<Token> stack, bool isNotElementOfArray)
{
	int index = isNotElementOfArray ? 4 : 7;
	Token idToken = stack.at(stack.size() - index);
	Token expressionAndSymbolToken = stack.at(stack.size() - 2);
	bool isVarExist = false;

	for (Variable var : *m_variablesTable)
	{
		if (var.m_name == idToken.value)
		{
			isVarExist = true;
			break;
		}
	}

	if (!isVarExist) ErrorHandler::FailedWithNotExistingVariable(idToken.symbol.name, idToken.lineNumber);

	if (expressionAndSymbolToken.value != RuleName::EXPRESSION_AND_SYMBOL()) {

	}
}

int VariablesTable::getOffset(string varName) 
{	
	int offset = 0;
	if (m_variablesTable ->size()) 
	{
		for (Variable var : *m_variablesTable )
		{			
			if (var.m_name == varName)
			{				
				break;
			}
			offset = offset + var.m_size;
		}
	}	
	return offset;
}
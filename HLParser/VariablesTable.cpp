#include "stdafx.h"
#include "VariablesTable.h"
#include "SizeManager.h"
#include "TypeChecker.h"
#include "ErrorHandler.h"
#include "RuleName.h"
#include "ReservedWords.h"
#include <stack>

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

vector<string> VariablesTable::GetExpressionStack(vector<Token> stack)
{
	Token *currToken = &stack.back();
	std::stack<Token*> nextTokens;
	vector<string> expressionStack;

	do 
	{
		if (currToken->formingTokens.size() == 1)
		{
			currToken = &currToken->formingTokens.back();
		}
		else
		{
			if (currToken->formingTokens.size() == 3)
			{
				if (currToken->formingTokens.front().value == ")" && currToken->formingTokens.back().value == "(")
				{
					currToken = &currToken->formingTokens[1];
				}
				else
				{
					if (currToken->formingTokens.front().formingTokens.size() == 1)
					{
						expressionStack.push_back(currToken->formingTokens[1].value);
						nextTokens.push(&currToken->formingTokens.back());
						currToken = &currToken->formingTokens.front();
					}
					else if (currToken->formingTokens.front().formingTokens.size() == 3)
					{
						expressionStack.push_back(currToken->formingTokens[1].value);
						nextTokens.push(&currToken->formingTokens.front());
						currToken = &currToken->formingTokens.back();
					}
				}
			}
		}

		if (currToken->formingTokens.size() == 0)
		{
			expressionStack.push_back(currToken->value);
			if (nextTokens.size())
			{
				currToken = nextTokens.top();
				nextTokens.pop();
			}
		}
	} 
	while (currToken->formingTokens.size());

	reverse(expressionStack.begin(), expressionStack.end());

	return expressionStack;
}
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
	string help_type;
	int factor = 1;
	int xDimention = 0;

	if (stack.at(stack.size() - 3).formingTokens.back().symbol.name == RuleName::PRIMITIVE_TYPE())
	{
		typeToken = stack.at(stack.size() - 3);
		type = typeToken.formingTokens.front().formingTokens.front().value;
		help_type = type;
	}
	else
	{
		typeToken = stack.at(stack.size() - 3).formingTokens.back().formingTokens.back();
		type = typeToken.formingTokens.front().value;
		help_type = type;
		Token factorToken = stack.at(stack.size() - 3).formingTokens.back().formingTokens[1];
		if (factorToken.formingTokens.size() == 1)
		{
			help_type = type + "_array";
			factor = stoi(factorToken.formingTokens.back().formingTokens.back().value);
		}
		else
		{
			help_type = type + "_double_array";
			factor = stoi(factorToken.formingTokens.back().formingTokens.back().value) * stoi(factorToken.formingTokens.front().formingTokens.back().value);
			xDimention = stoi(factorToken.formingTokens.back().formingTokens.back().value);
		}
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
		variable.m_type = help_type;
		variable.m_size = SizeManager::SizeOfType(variable.m_type) * factor;
		variable.m_xDimension = xDimention;

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

bool VariablesTable::DoesVariableExists(string variablesName)
{
	for (Variable variable : *m_variablesTable)
	{
		if (variable.m_name == variablesName)
		{
			return true;
		}
	}
	return false;
}

string VariablesTable::getType(string varName)
{
	string type = "";

	if (m_variablesTable->size())
	{
		for (Variable var : *m_variablesTable)
		{
			if (var.m_name == varName)
			{
				type += var.m_type;
				break;
			}
		}
	}
	return type;
}

int VariablesTable::getXDimention(string varName)
{
	int x = 0;

	if (m_variablesTable->size())
	{
		for (Variable var : *m_variablesTable)
		{
			if (var.m_name == varName)
			{
				x = var.m_xDimension;
				break;
			}
		}
	}
	return x;
}

int VariablesTable::getSize(string varName)
{
	if (m_variablesTable->size())
	{
		for (Variable var : *m_variablesTable)
		{
			if (var.m_name == varName)
			{
				return var.m_size;
			}
		}
	}
	return -1;
}

int VariablesTable::getOffset(string varName)
{
	int offset = 0;

	if (m_variablesTable->size())
	{
		for (Variable var : *m_variablesTable)
		{
			if (var.m_name == varName)
			{
				offset += +var.m_size;
				break;
			}
			offset = offset + var.m_size;
		}
	}
	return offset;
}


vector<string> VariablesTable::GetExpressionStack(vector<Token> stack, bool isAssignment)
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
		if (currToken->formingTokens.size() == 2)
		{
			if (currToken->formingTokens.back().value == "!")
			{
				expressionStack.push_back("!");
			}
			else
			{
				expressionStack.push_back("--");
			}
			currToken = &currToken->formingTokens.front();
		}
		else
		{
			if (currToken->formingTokens.size() == 4)
			{
				if (currToken->formingTokens.back().value == "!")
				{
					expressionStack.push_back("!");
				}
				nextTokens.push(&currToken->formingTokens[1]);
				currToken = &currToken->formingTokens.back();
			}
			if (currToken->formingTokens.size() == 3)
			{
				if (currToken->formingTokens.front().value == ")" && currToken->formingTokens.back().value == "(")
				{
					currToken = &currToken->formingTokens[1];
				}
				if (currToken->formingTokens.size() > 1 && currToken->formingTokens[1].value == ",")
				{
					nextTokens.push(&currToken->formingTokens.back());
					currToken = &currToken->formingTokens.front();
				}
				else
				{
					if (currToken->formingTokens.front().formingTokens.size() == 1)
					{
						if (currToken->formingTokens.size() > 1) {
							expressionStack.push_back(currToken->formingTokens[1].value);
							nextTokens.push(&currToken->formingTokens.back());
						}
						currToken = &currToken->formingTokens.front();
					}
					else if (currToken->formingTokens.front().formingTokens.size() == 2)
					{
						expressionStack.push_back(currToken->formingTokens[1].value);
						nextTokens.push(&currToken->formingTokens.front());
						currToken = &currToken->formingTokens.back();
					}
					else if (currToken->formingTokens.front().formingTokens.size() == 3)
					{
						if (currToken->formingTokens.size() == 3)
						{
							expressionStack.push_back(currToken->formingTokens[1].value);
							nextTokens.push(&currToken->formingTokens.back());
							currToken = &currToken->formingTokens.front();
						}
						else
						{
							expressionStack.push_back(currToken->formingTokens.back().formingTokens[1].value);
							nextTokens.push(&currToken->formingTokens.back().formingTokens.front());
							currToken = &currToken->formingTokens.back().formingTokens.back();
						}
					}
				}
			}
		}

		if (currToken->formingTokens.size() == 0)
		{
			if (currToken->symbol.name == RuleName::IDENTIFIER() && !DoesVariableExists(currToken->value))
			{
				ErrorHandler::FailedWithNotExistingVariable(currToken->value, currToken->lineNumber);
			}
			expressionStack.push_back(currToken->value);
			if (nextTokens.size())
			{
				currToken = nextTokens.top();
				nextTokens.pop();
			}

		}
	} while (currToken->formingTokens.size());

	if (isAssignment)
		CompareTypes(stack, expressionStack);
	reverse(expressionStack.begin(), expressionStack.end());

	return expressionStack;
}

Token *VariablesTable::GetRootToken(Token *token)
{
	while (token->formingTokens.size())
	{
		token = &token->formingTokens.back();
	}

	return token;
}

void VariablesTable::CompareTypes(vector<Token> stack, vector<string> rightPart)
{
	Token *leftPart = NULL;
	bool isElementOfArray = false;
	bool is2DArray = false;

	if (stack.at(stack.size() - 3).symbol.name == RuleName::IDENTIFIER())
	{
		leftPart = &stack.at(stack.size() - 3);
	}
	else
	{
		leftPart = &stack.at(stack.size() - 6);
		isElementOfArray = true;
		if (stack.at(stack.size() - 4).formingTokens.size() == 3) {
			is2DArray = true;
		}
	}

	if (isElementOfArray && !is2DArray)
	{

	}
	if (isElementOfArray && is2DArray)
	{

	}
	if (!isElementOfArray)
	{
		string type = getType(leftPart->value);
		if (type == "int" || type == "bool")
		{
			if (!IsAssignableIntBoolExpression(rightPart, leftPart))
			{
				ErrorHandler::FailedWithNotAssignableIntBoolExpression(leftPart->value, leftPart->lineNumber);
			}
		}
		else
		{
			if (type == "int_array" || type == "bool_array" || type == "int_double_array" || type == "bool_double_array")
			{
				if (!IsAssignableArrayExpression(rightPart, leftPart))
				{
					ErrorHandler::FailedWithNotAssignableArrayExpression(leftPart->value, leftPart->lineNumber);
				}
			}
		}
	}
}

bool VariablesTable::IsAssignableArrayExpression(vector<string> stack, Token *arrayToken)
{
	if (stack.size() == 1)
	{
		if (getType(arrayToken->value) != getType(stack.back()))
		{
			return false;
		}
		else
		{
			if (getSize(arrayToken->value) != getSize(stack.back()))
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool VariablesTable::IsAssignableIntBoolExpression(vector<string> stack, Token *idToken)
{
	if (stack.size() == 1)
	{
		if (getType(idToken->value) == "int" && getType(stack.back()) == "int")
		{
			return true;
		}
		if (getType(idToken->value) == "int" && getType(stack.back()) == "bool")
		{
			return true;
		}
		if (getType(idToken->value) == "bool" && getType(stack.back()) == "bool")
		{
			return true;
		}
		if (getType(idToken->value) == "bool" && getType(stack.back()) == "int")
		{
			return true;
		}
		if (getSize(idToken->value) != getSize(stack.back()))
		{
			return false;
		}
	}

	return true;
}
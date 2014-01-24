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
	if (token->nodes.size())
	{
		token = GetLastVariable(&token->nodes[0]);
	}
	else
	{
		return token;
	}
}

void VariablesTable::GetBodyWithoutTail(Token & token)
{
	if (token.nodes.size() == 3)
	{
		GetBodyWithoutTail(token.nodes[0]);
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

	if (stack.at(stack.size() - 3).nodes.back().symbol.m_term == RuleName::PRIMITIVE_TYPE())
	{
		typeToken = stack.at(stack.size() - 3);
		type = typeToken.nodes.front().nodes.front().lexeme;
		help_type = type;
	}
	else
	{
		typeToken = stack.at(stack.size() - 3).nodes.back().nodes.back();
		type = typeToken.nodes.front().lexeme;
		help_type = type;
		Token factorToken = stack.at(stack.size() - 3).nodes.back().nodes[1];
		if (factorToken.nodes.size() == 1)
		{
			help_type = type + "_array";
			factor = stoi(factorToken.nodes.back().nodes.back().lexeme);
		}
		else
		{
			help_type = type + "_double_array";
			factor = stoi(factorToken.nodes.back().nodes.back().lexeme) * stoi(factorToken.nodes.front().nodes.back().lexeme);
			xDimention = stoi(factorToken.nodes.back().nodes.back().lexeme);
		}
	}

	if (!TypeChecker::IsAllowedType(type))
	{
		ErrorHandler::FailedWithTypeError(type, typeToken.line);
	}

	Token *identifiersDefinition = &stack.at(stack.size() - 2);
	while (identifiersDefinition) {
		Token *idToken = NULL;
		Token *identifierDefinition;
		Variable variable;
		int count = 1;

		identifierDefinition = &identifiersDefinition->nodes.back();
		if (identifierDefinition->nodes.size() == 1)
		{
			idToken = &identifierDefinition->nodes.front();
		}
		else
		{
			idToken = identifierDefinition;
		}

		if (ReservedWords::IsReservedWord(idToken->lexeme))
		{
			ErrorHandler::FailedWithReservedWord(idToken->lexeme, idToken->line);
		}

		if (m_variablesTable->size())
		{
			for (Variable var : *m_variablesTable)
			{
				if (var.m_name == idToken->lexeme)
				{
					ErrorHandler::FailedWithRedefinition(idToken->lexeme, idToken->line);
				}
			}
		}

		variable.m_name = idToken->lexeme;
		variable.m_type = help_type;
		variable.m_size = SizeManager::SizeOfType(variable.m_type) * factor;
		variable.m_xDimension = xDimention;

		m_variablesTable->push_back(variable);

		if (identifiersDefinition->nodes.size() == 3)
		{
			identifiersDefinition = &identifiersDefinition->nodes.front();
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
		if (var.m_name == idToken.lexeme)
		{
			isVarExist = true;
			break;
		}
	}

	if (!isVarExist) ErrorHandler::FailedWithNotExistingVariable(idToken.symbol.m_term, idToken.line);

	if (expressionAndSymbolToken.lexeme != RuleName::EXPRESSION_AND_SYMBOL()) {

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
		if (currToken->nodes.size() == 1)
		{
			currToken = &currToken->nodes.back();
		}
		else
		if (currToken->nodes.size() == 2)
		{
			if (currToken->nodes.back().lexeme == "!")
			{
				expressionStack.push_back("!");
			}
			else
			{
				expressionStack.push_back("--");
			}
			currToken = &currToken->nodes.front();
		}
		else
		{
			if (currToken->nodes.size() == 4)
			{
				if (currToken->nodes.back().lexeme == "!")
				{
					expressionStack.push_back("!");
				}
				nextTokens.push(&currToken->nodes[1]);
				currToken = &currToken->nodes.back();
			}
			if (currToken->nodes.size() == 3)
			{
				if (currToken->nodes.front().lexeme == ")" && currToken->nodes.back().lexeme == "(")
				{
					currToken = &currToken->nodes[1];
				}
				if (currToken->nodes.size() > 1 && currToken->nodes[1].lexeme == ",")
				{
					nextTokens.push(&currToken->nodes.back());
					currToken = &currToken->nodes.front();
				}
				else
				{
					if (currToken->nodes.front().nodes.size() == 1)
					{
						if (currToken->nodes.size() > 1) {
							expressionStack.push_back(currToken->nodes[1].lexeme);
							nextTokens.push(&currToken->nodes.back());
						}
						currToken = &currToken->nodes.front();
					}
					else if (currToken->nodes.front().nodes.size() == 2)
					{
						expressionStack.push_back(currToken->nodes[1].lexeme);
						nextTokens.push(&currToken->nodes.front());
						currToken = &currToken->nodes.back();
					}
					else if (currToken->nodes.front().nodes.size() == 3)
					{
						if (currToken->nodes.size() == 3)
						{
							expressionStack.push_back(currToken->nodes[1].lexeme);
							nextTokens.push(&currToken->nodes.back());
							currToken = &currToken->nodes.front();
						}
						else
						{
							expressionStack.push_back(currToken->nodes.back().nodes[1].lexeme);
							nextTokens.push(&currToken->nodes.back().nodes.front());
							currToken = &currToken->nodes.back().nodes.back();
						}
					}
				}
			}
		}

		if (currToken->nodes.size() == 0)
		{
			if (currToken->symbol.m_term == RuleName::IDENTIFIER() && !DoesVariableExists(currToken->lexeme))
			{
				ErrorHandler::FailedWithNotExistingVariable(currToken->lexeme, currToken->line);
			}
			expressionStack.push_back(currToken->lexeme);
			if (nextTokens.size())
			{
				currToken = nextTokens.top();
				nextTokens.pop();
			}

		}
	} while (currToken->nodes.size());

	if (isAssignment)
		CompareTypes(stack, expressionStack);
	reverse(expressionStack.begin(), expressionStack.end());

	return expressionStack;
}

Token *VariablesTable::GetRootToken(Token *token)
{
	while (token->nodes.size())
	{
		token = &token->nodes.back();
	}

	return token;
}

void VariablesTable::CompareTypes(vector<Token> stack, vector<string> rightPart)
{
	Token *leftPart = NULL;
	bool isElementOfArray = false;
	bool is2DArray = false;

	if (stack.at(stack.size() - 3).symbol.m_term == RuleName::IDENTIFIER())
	{
		leftPart = &stack.at(stack.size() - 3);
	}
	else
	{
		leftPart = &stack.at(stack.size() - 6);
		isElementOfArray = true;
		if (stack.at(stack.size() - 4).nodes.size() == 3) {
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
		string type = getType(leftPart->lexeme);
		if (type == "int" || type == "bool")
		{
			if (!IsAssignableIntBoolExpression(rightPart, leftPart))
			{
				ErrorHandler::FailedWithNotAssignableIntBoolExpression(leftPart->lexeme, leftPart->line);
			}
		}
		else
		{
			if (type == "int_array" || type == "bool_array" || type == "int_double_array" || type == "bool_double_array")
			{
				if (!IsAssignableArrayExpression(rightPart, leftPart))
				{
					ErrorHandler::FailedWithNotAssignableArrayExpression(leftPart->lexeme, leftPart->line);
				}
			}
		}
	}
}

bool VariablesTable::IsAssignableArrayExpression(vector<string> stack, Token *arrayToken)
{
	if (stack.size() == 1)
	{
		if (getType(arrayToken->lexeme) != getType(stack.back()))
		{
			return false;
		}
		else
		{
			if (getSize(arrayToken->lexeme) != getSize(stack.back()))
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
		if (getType(idToken->lexeme) == "int" && getType(stack.back()) == "int")
		{
			return true;
		}
		if (getType(idToken->lexeme) == "int" && getType(stack.back()) == "bool")
		{
			return true;
		}
		if (getType(idToken->lexeme) == "bool" && getType(stack.back()) == "bool")
		{
			return true;
		}
		if (getType(idToken->lexeme) == "bool" && getType(stack.back()) == "int")
		{
			return true;
		}
		if (getSize(idToken->lexeme) != getSize(stack.back()))
		{
			return false;
		}
	}

	return true;
}
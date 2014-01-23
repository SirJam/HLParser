#pragma once

#include <string>
using namespace std;

class RuleName
{
public:
	static const string GOAL()
	{
		return "Goal";
	}

	static const string NONTERMINAL()
	{
		return "nonTerminal";
	}
	static const string EOF_RULE()
	{
		return "EOF";
	}
	static const string ERROR()
	{
		return "Error";
	}
	static const string WHITESPACE()
	{
		return "Whitespace";
	}
	static const string DIGITS()
	{
		return "digits";
	}


	static const string IDENTIFIER_DEFINITION()
	{
		return "identifier_definition";
	}
	static const string IDENTIFIER_HANDLER()
	{
		return "identifier_handler";
	}
	static const string IDENTIFIER()
	{
		return "identifier";
	}
	static const string DEFINITION()
	{
		return "definition";
	}


	static const string STATEMENT()
	{
		return "statement";
	}
	static const string WRITE()
	{
		return "write";
	}
	static const string READ()
	{
		return "read";
	}
	static const string WHILE()
	{
		return "while";
	}


	static const string IF()
	{
		return "if";
	}
	static const string ELSE()
	{
		return "else";
	}
	static const string IF_CONSTRUCTION()
	{
		return "if_construction";
	}
	static const string ELSE_CONSTRUCTION()
	{
		return "else_construction";
	}


	static const string EXPRESSION_AND_SYMBOL()
	{
		return "expression_and_symbol";
	}
	static const string EXPRESSION_6()
	{
		return "expression_6";
	}
	static const string EXPRESSION_5()
	{
		return "expression_5";
	}
	static const string EXPRESSION_4()
	{
		return "expression_4";
	}
	static const string EXPRESSION_3()
	{
		return "expression_3";
	}
	static const string EXPRESSION_2()
	{
		return "expression_2";
	}
	static const string EXPRESSION_1()
	{
		return "expression_1";
	}
	static const string EXPRESSION_0()
	{
		return "expression_0";
	}

	static const string PRIMITIVE_TYPE()
	{
		return "primitive_type";
	}
};
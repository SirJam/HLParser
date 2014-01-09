#pragma once

#include <string>
using namespace std;

class RuleName
{
public:
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


	static const string EXPRESSION_AND_SYMBOL()
	{
		return "expression_and_symbol";
	}


	static const string PRIMITIVE_TYPE()
	{
		return "primitive_type";
	}
};
#pragma once

#include <string>
#include <sstream>
using namespace std;

class ErrorHandler
{
public:
	static string FailedWithTypeError(string type)
	{
		return "Type '" + type + "' is not allowed!\n";
	}
	static string FailedWithRedefinition(string name)
	{
		return "Redefinition of variable '" + name + "'!\n";
	}
	static string FailedWithReservedWord(string name)
	{
		return "Name '" + name + "' is not allowed because it is reserved word!\n";
	}
	static string FailedWithNotExistingVariable(string name)
	{
		return "Variable '" + name + "' has not been declared!\n";
	}
	static string FailedWithNotAssignableArrayExpression(string name)
	{
		return "'" + name + "' array can only assign it an array of similar type and size!\n";
	}
	static string FailedWithNotAssignableIntBoolExpression(string name)
	{
		return "'" + name + "' variable can only be assigned to variable of similar type and size!\n";
	}
	static string FailedWithTokenError(string expectedValues, string name)
	{
		return "\n  --==Syntax error==--\n\nExpected tokens: \n\n" + expectedValues + "\n\nReceived token: '" + name + "'!\n";
	}
	static string FailedWithMaxLengthOfNumber(string name)
	{
		return "Value of number '" + name + "' is more than 999.999.999\n";
	}

	static void ShowErrorLineAndMessageAndExit(string str)
	{
		cout << str;
		exit(EXIT_FAILURE);
	}

	static const string ErrorAtLine(int line)
	{
		stringstream out;
		out << "Error at line " << line << endl;
		return out.str();
	}
};

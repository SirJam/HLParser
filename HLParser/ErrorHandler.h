#pragma once

#include <string>
#include <sstream>
using namespace std;

class ErrorHandler
{
public:
	static void FailedWithTypeError(string type, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "Type '" + type + "' is not allowed!\n");
	}
	static void FailedWithRedefinition(string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "Redefinition of variable '" + name + "'!\n");
	}
	static void FailedWithReservedWord(string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "Name '" + name + "' is not allowed because it is reserved word!\n");
	}
	static void FailedWithNotExistingVariable(string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "Variable '" + name + "' has not been declared!\n");
	}
	static void FailedWithNotAssignableArrayExpression(string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "'" + name + "' array can only assign it an array of similar type and size!\n");
	}
	static void FailedWithNotAssignableIntBoolExpression(string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "'" + name + "' variable can only be assigned to variable of similar type and size!\n");
	}
	static void FailedWithTokenError(string expectedValues, string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "\n  --==Syntax error==--\n\n" + "Expected tokens: \n\n" + expectedValues + "\nReceived token: '" + name + "'!\n");
	}
	static void FailedWithMaxLengthOfNumber(string name, int line)
	{
		ShowErrorLineAndMessageAndExit(ErrorAtLine(line) + "Value of number '" + name + "' is more than 999.999.999\n");
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

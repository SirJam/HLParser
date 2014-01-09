#pragma once

#include <string>
#include <sstream>
using namespace std;

class ErrorHandler
{
public:
	static void failWithErrorCode(int const code, string const& data, int const lineNumber);
	static void failWithErrorCode(int const code, string const& data1, string const& data2, int const lineNumber);

	static void FailedWithTypeError(string type, int line)
	{
		cout << ErrorAtLine(line);
		cout << "Type " << type << " is not allowed!" << endl;
		exit(EXIT_FAILURE);
	}
	static void FailedWithRedefinition(string variableName, int line)
	{
		cout << ErrorAtLine(line);
		cout << "Redefinition of variable " << variableName << "!" << endl;
		exit(EXIT_FAILURE);
	}
	static void FailedWithReservedWord(string name, int line)
	{
		cout << ErrorAtLine(line);
		cout << "Name " << name << " is not allowed because it is reserved word!" << endl;
		exit(EXIT_FAILURE);
	}


	static const string ErrorAtLine(int line)
	{
		stringstream out;
		out << "Error at line " << line << endl;
		return out.str();
	}
private:
	static void failWithError(string const& error, int const lineNumber);

	static void failWithScannerError(int const code, string const& data1, string const& data2, int const lineNumber);
	static void failWithParserError(int const code, string const& data1, string const& data2, int const lineNumber);
};

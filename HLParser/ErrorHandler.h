#pragma once

#include <string>
using namespace std;

class ErrorHandler
{
public:
	static void failWithErrorCode(int const code, string const& data, int const lineNumber);
	static void failWithErrorCode(int const code, string const& data1, string const& data2, int const lineNumber);
private:
	static void failWithError(string const& error, int const lineNumber);

	static void failWithScannerError(int const code, string const& data1, string const& data2, int const lineNumber);
	static void failWithParserError(int const code, string const& data1, string const& data2, int const lineNumber);
};

#include "StdAfx.h"
#include "ErrorHandler.h"
#include <sstream>
#include <iostream>

using namespace std;
using std::ostringstream;

void ErrorHandler::failWithError(string const& error, int const lineNumber)
{
	cerr << error;
	if (lineNumber) {
		cerr << " at line " << lineNumber;
	}
	cerr << endl;
	exit(EXIT_FAILURE);
}

void ErrorHandler::failWithErrorCode(int const code, string const& data, int const lineNumber)
{
	failWithErrorCode(code, data, "", lineNumber);
}

void ErrorHandler::failWithErrorCode(int const code, string const& data1, string const& data2, int const lineNumber)
{
	/*
		Error codes:
			0xx - Scanner errors
			1xx - Parser errors
			3xx - Variable errors
			4xx - Function errors
			5xx - Type errors
	*/

	int const errorBase = 100;
	int const errorType = code / errorBase;
	int const errorDetail = code % errorBase;

	switch (errorType) {
		case 0:
			failWithScannerError(errorDetail, data1, data2, lineNumber);
			break;
		default:
			break;
	}
}

void ErrorHandler::failWithScannerError(int const code, string const& data1, string const& data2, int const lineNumber)
{
	/*
		Error codes:
			001 - File ... not found
			002 - Multiline comment mismatch
			003 - Expected ... keyword but got ...
			004 - Define word ... doesn't exist
	*/

	ostringstream errorStream;

	switch (code) {
		case 1:
			errorStream << "File '" << data1 << "' not found.";
			break;
		case 2:
			errorStream << "Multiline comment mismatch.";
			break;
		case 3:
			errorStream << "Expected '" << data1 << "' keyword but got '" << data2 << "'.";
			break;
		case 4:
			errorStream << "Define word '" << data1 << "' doesn't exist.";
			break;
		default:
			errorStream << "Scanner error with code " << code << " doesn't exist."; 
			break;
	}

	failWithError(errorStream.str(), lineNumber);
}

void ErrorHandler::failWithParserError(int const code, string const& data1, string const& data2, int const lineNumber)
{
	ostringstream errorStream;

	switch (code) {
		case 1:
			errorStream << "Syntax error:\n" << "expected: " << data1 << "but got '" << data2 << "'";
			break;
		default:
			break;
	}

	failWithError(errorStream.str(), lineNumber);
}

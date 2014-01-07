#pragma once

enum TYPE
{
	TYPE_INT,
	TYPE_BOOL,
	TYPE_CHAR,
};

string inline typeAsString(TYPE const type)
{
	string typeString = "";
	switch (type) {
		case TYPE_INT:
			typeString = "int";
			break;
		case TYPE_BOOL:
			typeString = "bool";
			break;
		case TYPE_CHAR:
			typeString = "char";
			break;
	}
	return typeString;
}

TYPE inline stringAsType(string const& typeString)
{
	TYPE type;

	if (typeString == "int") {
		type = TYPE_INT;
	}
	else if (typeString == "bool") {
		type = TYPE_BOOL;
	}
	else if (typeString == "char") {
		type = TYPE_CHAR;
	}
	return type;
}
#pragma once
#include "tinyxml.h"
#include "Symbol.h"
#include "Production.h"
#include "Action.h";

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TablesReader
{
private:
	void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0);
	ofstream outfile;

	int m_lalrStateIndex;
	int m_currentProduction;
	vector<int> m_handlesForCurrentProduction;

	vector<Symbol> *m_symbolsTable;
	vector<Production> *m_productionsTable;
	vector<Action> *m_actionsTable;

	void GetSymbol(TiXmlElement* pElement);
	void GetProduction(TiXmlElement* pElement);
	void GetLALRAction(TiXmlElement* pElement);
	void GetLALRStateIndex(TiXmlElement* pElement);
	void GetHandles(TiXmlElement* pElement);
	void SetHandlesForCurrentProduction();
	
public:
	TablesReader();
	~TablesReader();

	vector<Symbol> *GetSymbolsTable();
	vector<Production> *GetProductionsTable();
	vector<Action> *GetActionsTable();

	void dump_to_stdout(const char* pFilename);
};


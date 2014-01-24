#include "stdafx.h"
#include "TablesReader.h"

TablesReader::TablesReader()
{
	m_symbolsTable = new vector<Symbol>;
	m_productionsTable = new vector<Production>;
	m_actionsTable = new vector<Action>;
	SetHandlesForCurrentProduction();

	m_lalrStateIndex = 0;
	m_currentProduction = 0;
	dump_to_stdout("HLGrammar.xml");
}

vector<Symbol> *TablesReader::GetSymbolsTable()
{
	return m_symbolsTable;
}

vector<Production> *TablesReader::GetProductionsTable()
{
	return m_productionsTable;
}

vector<Action> *TablesReader::GetActionsTable()
{
	return m_actionsTable;
}

void TablesReader::GetSymbol(TiXmlElement* pElement)
{
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	Symbol symbol;

	while (pAttrib)
	{
		if (pAttrib->NameTStr() == "Index")
		{
			symbol.m_index = pAttrib->IntValue();
		} 
		else
		{
			if (pAttrib->NameTStr() == "Name")
			{
				symbol.m_term = pAttrib->Value();
			}
			else
			{
				if (pAttrib->NameTStr() == "Type")
				{
					symbol.m_type = pAttrib->IntValue();
				}
			}
		}
		pAttrib = pAttrib->Next();
	}

	m_symbolsTable->push_back(symbol);
}

void TablesReader::GetProduction(TiXmlElement* pElement)
{
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	Production production;

	while (pAttrib)
	{
		if (pAttrib->NameTStr() == "Index")
		{
			production.index = pAttrib->IntValue();
			m_currentProduction = production.index;
		}
		else
		{
			if (pAttrib->NameTStr() == "NonTerminalIndex")
			{
				production.nonTerminalIndex = pAttrib->IntValue();
			}
		}

		pAttrib = pAttrib->Next();
	}

	m_productionsTable->push_back(production);
}

void TablesReader::GetLALRAction(TiXmlElement* pElement)
{
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	Action action;
	action.stateIndex = m_lalrStateIndex;

	while (pAttrib)
	{
		if (pAttrib->NameTStr() == "SymbolIndex") 
		{
			action.symbolIndex = pAttrib->IntValue();
		}
		else
		{
			if (pAttrib->NameTStr() == "Action")
			{
				action.type = pAttrib->IntValue();
			}
			else
			{
				if (pAttrib->NameTStr() == "Value")
				{
					action.target = pAttrib->IntValue();
				}
			}
		}

		pAttrib = pAttrib->Next();
	}

	m_actionsTable->push_back(action);
}

void TablesReader::GetLALRStateIndex(TiXmlElement* pElement)
{
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	
	while (pAttrib)
	{
		if (pAttrib->NameTStr() == "Index") {
			m_lalrStateIndex = pAttrib->IntValue();
		}
		pAttrib = pAttrib->Next();
	}
}

void TablesReader::GetHandles(TiXmlElement* pElement)
{
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();

	while (pAttrib)
	{
		if (pAttrib->NameTStr() == "SymbolIndex") {
			m_handlesForCurrentProduction.push_back(pAttrib->IntValue());
		}
		pAttrib = pAttrib->Next();
	}
}

void TablesReader::SetHandlesForCurrentProduction()
{
	for (int i = 0; i < m_productionsTable->size(); i++)
	{
		if (m_productionsTable->at(i).index == m_currentProduction)
		{
			m_productionsTable->at(i).handles = m_handlesForCurrentProduction;
		}
	}

	m_handlesForCurrentProduction.clear();
}

void TablesReader::dump_to_stdout(TiXmlNode* pParent, unsigned int indent)
{
	if (!pParent) return;

	TiXmlNode* pChild;
	int t = pParent->Type();
	
	if (t == TiXmlNode::TINYXML_ELEMENT)
	{
		if (pParent->ValueTStr() == "Symbol")
		{
			GetSymbol(pParent->ToElement());
		}
		else
		{
			if (pParent->ValueTStr() == "Production")
			{
				if (m_handlesForCurrentProduction.size())
				{
					SetHandlesForCurrentProduction();
				}
				GetProduction(pParent->ToElement());
			}
			else
			{
				if (pParent->ValueTStr() == "ProductionSymbol")
				{
					GetHandles(pParent->ToElement());
				}
				else
				{
					if (pParent->ValueTStr() == "LALRState")
					{
						GetLALRStateIndex(pParent->ToElement());
					}
					if (pParent->ValueTStr() == "LALRAction")
					{
						GetLALRAction(pParent->ToElement());
					}
				}
			}
		}
	}
	
	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		dump_to_stdout(pChild, indent + 1);
	}
}

// load the named file and dump its structure to STDOUT
void TablesReader::dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		dump_to_stdout(&doc); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}

TablesReader::~TablesReader()
{
}

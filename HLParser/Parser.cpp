#include "StdAfx.h"
#include "Parser.h"
#include "Symbol.h"
#include "ErrorHandler.h"
#include "TablesReader.h"

Parser::Parser(Generator *generator)
:generator(generator)
{
	TablesReader *tablesReader = new TablesReader();

	m_symbolsTable = tablesReader->GetSymbolsTable();
	m_productionsTable = tablesReader->GetProductionsTable();
	m_actionsTable = tablesReader->GetActionsTable();
	
	tablesReader->~TablesReader();

	m_variablesTable = new VariablesTable();

	m_states.push_back(0);
	m_tokens.push_back(createTokenEOF());
}

Token Parser::createTokenEOF()
{
	Symbol symbol;
	symbol.index = 0;
	symbol.name = "EOF";
	symbol.type = 3;

	int lineNumber = m_tokens.size() ? m_tokens.back().lineNumber : 0;

	return Token(symbol, "EOF", lineNumber);
}

Symbol Parser::symbolWithIndex(int index)
{
	for (Symbol symbol : *m_symbolsTable) {
		if (symbol.index == index) return symbol;
	}
}

void Parser::getNextToken(Token const& token)
{	
	//printState(token);
	for (vector<Action>::iterator it = m_actionsTable->begin(); it != m_actionsTable->end(); it++) {
		Action *action = &(*it);
		if (action->stateIndex == m_states.back() && action->symbolIndex == token.symbol.index) {
			switch (action->type) {
				case 1: {
					Shift(*action, token);
					return;
				}
				case 2: {
					if (Reduce(*action)) return;
				}
					break;
				case 4: {
					Accept();
					return;
				}
			}
		}
	}	

	HandleError(token);
}

void Parser::Shift(Action action, Token token)
{	
	m_tokens.push_back(token);
	m_states.push_back(action.target);
	lastAction = 1;	

	if (m_tokens.back().symbol.name == ")") {
		if (m_tokens.end()[-2].symbol.name == "expression_0" &&
			m_tokens.end()[-3].symbol.name == "(" &&
			m_tokens.end()[-4].symbol.name == "if") {
			string falseLabel;
			cout << "generate <if_start>" << endl;
			//generator->createIfExpressionStartPart(falseLabel);
			//createdIfExpressionsLabels.push_back(falseLabel);
			//cout << ("PUSH LABEL " + falseLabel + "\n");
		}
	}
}

void Parser::computeProduction(Production *production) 
{
	Symbol symbol = symbolWithIndex(production->nonTerminalIndex);	

	if (symbol.name == RuleName::IDENTIFIER_DEFINITION())
	{
		m_variablesTable->TryToRegisterVariable(m_tokens);
		//Token *token = &m_tokens.back();
		//cout << token->symbol.name << endl;
	}
	else if (symbol.name == "identifiers_definition")
	{
		cout << "setMemory" << endl;;
	}
	else if (symbol.name == "expression_3")
	{
		Token *addopToken = &m_tokens.end()[-2];
		Token *firstOperand = &m_tokens.end()[-3];
		Token *secondOperand = &m_tokens.end()[-1];			
		// Checking operand types

		string operation = addopToken->value;
			
		if (operation == ">") 
		{
			cout << ">" << endl;
		}
		else if (operation == "<")
		{
			cout << "<" << endl;
		}
		else if (operation == ">=")
		{
			cout << ">=" << endl;
		}
		else if (operation == "<=")
		{
			cout << "<=" << endl;
		}
	}
	else if (symbol.name == "expression_4") // + -
	{
		if (production->handles.size() == 3) 
		{			
			Token *addopToken = &m_tokens.end()[-2];
			Token *firstOperand = &m_tokens.end()[-3];
			Token *secondOperand = &m_tokens.end()[-1];			
			// Checking operand types

			string operation = addopToken->value;
			
			if (operation == "+") 
			{
				cout << "+" << endl;
				//generator->createAddOperation();
			}
			else if (operation == "-")
			{
				cout << "-" << endl;
				//generator->createSubstractOperation();
			}
		}
	}	
	else if (symbol.name == "expression_5") 
	{
		Token *addopToken = &m_tokens.end()[-2];
		Token *firstOperand = &m_tokens.end()[-3];
		Token *secondOperand = &m_tokens.end()[-1];			
		// Checking operand types

		string operation = addopToken->value;
			
		if (operation == "*") 
		{
			cout << "*" << endl;
			//generator->createMultiplyOperation();
		}
		else if (operation == "/")
		{
			cout << "/" << endl;
			//generator->createDivideOperation();
		}
	}
	else if (symbol.name == "Goal") // programm end
	{
		//clear stack
		generator->createProgramEnd();
	}
	else if (symbol.name == "statement") // programm end
	{		
		if (m_tokens.at(m_tokens.size() - 4).symbol.name == RuleName::IDENTIFIER())
		{
			m_variablesTable->CheckExistingOfVariable(m_tokens, true);
		}
		if (m_tokens.at(m_tokens.size() - 7).symbol.name == RuleName::IDENTIFIER())
		{
			m_variablesTable->CheckExistingOfVariable(m_tokens, false);
		}
		if (m_tokens.end()[-5].symbol.name == "write")
		{
			cout << "write" << endl;
		}
		if (m_tokens.end()[-5].symbol.name == "read")
		{
			cout << "read" << endl;
		}
		if (m_tokens.end()[-3].symbol.name == "=" && m_tokens.end()[-2].symbol.name != "]")
		{
			cout << "=" << endl;
		}
		if (m_tokens.end()[-3].symbol.name == "=" && m_tokens.end()[-2].symbol.name == "]")
		{
			cout << "]=" << endl;
		}
		if (m_tokens.end()[-7].symbol.name == "while")
		{
			cout << "while" << endl;
		}		
	}
	else if (symbol.name == "if_construction") {
		cout << "if_end" << endl;
	}
	else if (symbol.name == "else_construction") {
		cout << "else_construction" << endl;
	}
}



bool Parser::Reduce(Action action)
{
	for (vector<Production>::iterator it2 = m_productionsTable->begin(); it2 != m_productionsTable->end(); it2++) {
		Production *production = &(*it2);		
		if (production->index == action.target) {	
			//realize production
			computeProduction(production);
			// Removing right part of the production from stack
			vector<Token> formingTokens;
			for (vector<int>::reverse_iterator rit = production->handles.rbegin(); rit != production->handles.rend(); rit++) {
				int symbolIndex = (*rit);				
				if (symbolIndex == m_tokens.back().symbol.index) {
					formingTokens.push_back(m_tokens.back());
					m_tokens.pop_back();
					m_states.pop_back();
				}
			}
			// Adding left part of the production to stack
			Symbol symbolNonTerminal = symbolWithIndex(production->nonTerminalIndex);

			int lineNum = 0;
			for (vector<Token>::iterator it3 = formingTokens.begin(); it3 != formingTokens.end(); it3++) 
			{
				int tokenLineNumber = it3->lineNumber;
				if (tokenLineNumber) 
				{
					lineNum = tokenLineNumber;
				}
			}

			Token nonTerminalToken(symbolNonTerminal, "nonTerminal", lineNum);
			nonTerminalToken.formingTokens = formingTokens;
			m_tokens.push_back(nonTerminalToken);
			// Finding next state
			for (vector<Action>::iterator it4 = m_actionsTable->begin(); it4 != m_actionsTable->end(); it4++) 
			{
				Action *currentAction = &(*it4);
				if (currentAction->stateIndex == m_states.back() && currentAction->symbolIndex == m_tokens.back().symbol.index && currentAction->type == 3) 
				{
					m_states.push_back(currentAction->target);
					break;
				}
			}
			lastAction = 2;
			return true;
		}
	}
	return false;
}

void Parser::Accept()
{
	lastAction = 4;
}

void Parser::HandleError(Token token)
{
	string stream;
	for (Action action : *m_actionsTable) {
		if (action.stateIndex == m_states.back()) {
			stream += "'";
			stream += symbolWithIndex(action.symbolIndex).name; 
			stream += "' ";
		}
	}
	ErrorHandler::failWithErrorCode(101, stream, token.value, token.lineNumber);
}
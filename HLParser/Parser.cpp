#include "StdAfx.h"
#include "Parser.h"
#include "Symbol.h"
#include "ErrorHandler.h"
#include "TablesReader.h"

Parser::Parser()
{
	TablesReader *tablesReader = new TablesReader();

	m_symbolsTable = tablesReader->GetSymbolsTable();
	m_productionsTable = tablesReader->GetProductionsTable();
	m_actionsTable = tablesReader->GetActionsTable();
	
	tablesReader->~TablesReader();

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
}

bool Parser::Reduce(Action action)
{
	for (vector<Production>::iterator it2 = m_productionsTable->begin(); it2 != m_productionsTable->end(); it2++) 
	{
		Production *production = &(*it2);
		if (production->index == action.target) 
		{
			// Removing right part of the production from stack
			vector<Token> formingTokens;
			for (vector<int>::reverse_iterator rit = production->handles.rbegin(); rit != production->handles.rend(); rit++) 
			{
				int symbolIndex = (*rit);
				if (symbolIndex == m_tokens.back().symbol.index) 
				{
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
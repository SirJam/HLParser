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
	symbol.name = RuleName::EOF_RULE();
	symbol.type = 3;

	int lineNumber = m_tokens.size() ? m_tokens.back().lineNumber : 0;

	return Token(symbol, RuleName::EOF_RULE(), lineNumber);
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
		if (m_tokens.end()[-2].symbol.name == RuleName::EXPRESSION_0() &&
			m_tokens.end()[-3].symbol.name == "(" &&
			m_tokens.end()[-4].symbol.name == RuleName::IF()) {
			string falseLabel;			
			cout << "if_start" << endl;
			//generator->createIfExpressionStartPart(falseLabel);
			//createdIfExpressionsLabels.push_back(falseLabel);
			//cout << ("PUSH LABEL " + falseLabel + "\n");
		}
	}
}

void Parser::computeProduction(Production *production) 
{
	Symbol symbol = symbolWithIndex(production->nonTerminalIndex);	
	
	Token *token = &m_tokens.back();
	if (symbol.name == RuleName::IDENTIFIER_DEFINITION())
	{
		m_variablesTable->TryToRegisterVariable(m_tokens);				
	}
	else if (symbol.name == RuleName::EXPRESSION_AND_SYMBOL())
	{
		vector<string> stack = m_variablesTable->GetExpressionStack(m_tokens, true);
		for (int i = 0; i < stack.size(); i++)
		{
			if (stack[i] == "+") {
				generator->createAddOperation();
			}
			else if (stack[i] == "-") {
				generator->createSubstractOperation();
			}
			else if (stack[i] == "*") {
				generator->createMultiplyOperation();
			}
			else if (stack[i] == "/") {
				generator->createDivideOperation();
			}
			else if (isdigit(stack[i][0])) {
				generator->createIntConstant(atoi(stack[i].c_str()));
			}
			else if (stack[i] == ">" || stack[i] == "<" || 
					 stack[i] == ">=" || stack[i] == "<=" ||
					 stack[i] == "==" || stack[i] == "!=") {
						 generator->createRelExpression(stack[i]);
			}
			else if (stack[i] == "&&") {
				generator->applyAndExpression();
			}
			else if (stack[i] == "||") {
				generator->applyOrExpression();
			}
			else if (stack[i] == "TRUE") {
				generator->createIntConstant(1);
			}
			else if (stack[i] == "FALSE") {
				generator->createIntConstant(0);
			}
			else { //identifier
				int offset = m_variablesTable->getOffset(stack[i]);
				string type = m_variablesTable->getType(stack[i]);
				int x = m_variablesTable->getXDimention(stack[i]);
				generator->createIntVariable(offset, type, x);
			}
		}
	}
	else if (symbol.name == RuleName::DEFINITION())
	{
		int size = 0;
		if (m_variablesTable->m_variablesTable ->size()) 
		{
			for (Variable var : *m_variablesTable->m_variablesTable )
			{
				size = size + var.m_size;
			}
		}		
		generator->createVariableSpace(size); //allocate space
	}
	else if (symbol.name == RuleName::EXPRESSION_3())
	{
		Token *addopToken = &m_tokens.end()[-2];
		Token *firstOperand = &m_tokens.end()[-3];
		Token *secondOperand = &m_tokens.end()[-1];			
		// Checking operand types

		string operation = addopToken->value;
			
		if (operation == ">") 
		{
		}
		else if (operation == "<")
		{
		}
		else if (operation == ">=")
		{
		}
		else if (operation == "<=")
		{
		}
	}
	else if (symbol.name == RuleName::EXPRESSION_4()) // + -
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
			}
			else if (operation == "-")
			{
			}
		}
	}	
	else if (symbol.name == RuleName::EXPRESSION_5()) 
	{
		Token *addopToken = &m_tokens.end()[-2];
		Token *firstOperand = &m_tokens.end()[-3];
		Token *secondOperand = &m_tokens.end()[-1];			
		// Checking operand types

		string operation = addopToken->value;
			
		if (operation == "*") 
		{
		}
		else if (operation == "/")
		{
		}
	}
	else if (symbol.name == RuleName::GOAL()) // programm end
	{
		//clear stack
		int size = 0;
		if (m_variablesTable->m_variablesTable ->size()) 
		{
			for (Variable var : *m_variablesTable->m_variablesTable )
			{
				size = size + var.m_size;
			}
		}				
		generator->createProgramEnd(size);
	}
	else if (symbol.name == RuleName::STATEMENT()) // programm end
	{	
		if (m_tokens.at(m_tokens.size() - 4).symbol.name == RuleName::IDENTIFIER())
		{
			m_variablesTable->CheckExistingOfVariable(m_tokens, true);			
		}
		if (m_tokens.at(m_tokens.size() - 7).symbol.name == RuleName::IDENTIFIER())
		{
			m_variablesTable->CheckExistingOfVariable(m_tokens, false);
		}
		if (m_tokens.end()[-5].symbol.name == RuleName::WRITE())
		{
			generator->createPrintInteger();
		}
		if (m_tokens.end()[-5].symbol.name == RuleName::READ())
		{
			//cout << "read" << endl;
		}
		if (m_tokens.end()[-3].symbol.name == "=" && m_tokens.end()[-4].symbol.name != "]")
		{			
			generator->createAssignmentOperation(m_variablesTable->getOffset(m_tokens.end()[-4].value), m_variablesTable->getType(m_tokens.end()[-4].value));
		}
		if (m_tokens.end()[-3].symbol.name == "=" && m_tokens.end()[-4].symbol.name == "]")//либо одномерный, либо 2мерный массив
		{			
			Token index = m_tokens.end()[-5];
			if (index.formingTokens.size() == 1) //одномерный массив
			{
				vector<Token> expression_0;
				expression_0.push_back(index.formingTokens.back());
				string index = m_variablesTable->GetExpressionStack(expression_0, false)[0];
				generator->createIntConstant(atoi(index.c_str()));
				generator->createAssignmentOperation(m_variablesTable->getOffset(m_tokens.end()[-7].value), m_variablesTable->getType(m_tokens.end()[-7].value));				
			}
			else if (index.formingTokens.size() == 3) //двумерный массив
			{
				vector<Token> expression_0, expression_1;
				expression_0.push_back(index.formingTokens.front());
				expression_1.push_back(index.formingTokens.back());	
				//
				generator->createIntConstant(atoi(m_variablesTable->GetExpressionStack(expression_1, false)[0].c_str())); //первый индекс
				generator->createIntConstant(atoi(m_variablesTable->GetExpressionStack(expression_0, false)[0].c_str())); //второй индекс
				generator->createAssignmentOperation(m_variablesTable->getOffset(m_tokens.end()[-7].value), 
												     m_variablesTable->getType(m_tokens.end()[-7].value),
													 m_variablesTable->getXDimention(m_tokens.end()[-7].value));				
			}
		}		
		if (m_tokens.end()[-7].symbol.name == RuleName::WHILE())
		{
			//cout << "while" << endl;
		}		
	}
	else if (symbol.name == RuleName::IF_CONSTRUCTION()) {
		cout << "if_end" << endl;
	}
	else if (symbol.name == RuleName::ELSE_CONSTRUCTION()) {
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

			Token nonTerminalToken(symbolNonTerminal, RuleName::NONTERMINAL(), lineNum);
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
	string expectedValues;
	for (Action action : *m_actionsTable) {
		if (action.stateIndex == m_states.back()) {
			expectedValues += "    '";
			expectedValues += symbolWithIndex(action.symbolIndex).name;
			expectedValues += "'\n";
		}
	}
	ErrorHandler::FailedWithTokenError(expectedValues, token.value, token.lineNumber);
}
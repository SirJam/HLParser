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
	symbol.m_index = 0;
	symbol.m_term = RuleName::EOF_RULE();
	symbol.m_type = 3;

	int line = m_tokens.size() ? m_tokens.back().line : 0;

	return Token(symbol, RuleName::EOF_RULE(), line);
}

Symbol Parser::symbolWithIndex(int index)
{
	for (Symbol symbol : *m_symbolsTable) {
		if (symbol.m_index == index) return symbol;
	}
}

void Parser::getNextToken(Token const& token)
{	
	//printState(token);
	for (vector<Action>::iterator it = m_actionsTable->begin(); it != m_actionsTable->end(); it++) {
		Action *action = &(*it);
		if (action->stateIndex == m_states.back() && action->symbolIndex == token.symbol.m_index) {
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

	if (m_tokens.back().symbol.m_term == RuleName::ELSE()) {		
		string miggleLabel = generator->WriteIfMiddle(createdIfExpressionsLabels.back());	
		createdIfExpressionsLabels.pop_back();
		createdIfExpressionsLabels.push_back(miggleLabel);
	}
}

void Parser::parseExpression(vector<string> stack)
{
	for (int i = 0; i < stack.size(); i++)
	{
		if (stack[i] == "+") {
			generator->AddToAsm();
		}
		else if (stack[i] == "-") {
			generator->SubToAsm();
		}
		else if (stack[i] == "*") {
			generator->MulToAsm();
		}
		else if (stack[i] == "/") {
			generator->DivToAsm();
		}
		else if (stack[i] == "%") {
			generator->ModuloToAsm();
		}
		else if (stack[i] == "--") {
			generator->NagativVal();
		}
		else if (stack[i] == "!") {
			generator->InverseExpression();
		}
		else if (isdigit(stack[i][0])) {
			generator->WriteIntConst(atoi(stack[i].c_str()));
		}
		else if (stack[i] == ">" || stack[i] == "<" || 
					stack[i] == ">=" || stack[i] == "<=" ||
					stack[i] == "==" || stack[i] == "!=") {
						generator->WriteRelation(stack[i]);
		}
		else if (stack[i] == "&&") {
			generator->WriteAnd();
		}
		else if (stack[i] == "||") {
			generator->WriteOr();
		}
		else if (stack[i] == "TRUE") {
			generator->WriteIntConst(1);
		}
		else if (stack[i] == "FALSE") {
			generator->WriteIntConst(0);
		}
		else { //identifier
			int offset = m_variablesTable->getOffset(stack[i]);
			string type = m_variablesTable->getType(stack[i]);
			int x = m_variablesTable->getXDimention(stack[i]);
			generator->WriteIntVar(offset, type, x);
		}
	}
}

void Parser::computeProduction(Production *production) 
{
	Symbol symbol = symbolWithIndex(production->nonTerminalIndex);	
	
	Token *token = &m_tokens.back();
	if (symbol.m_term == RuleName::IDENTIFIER_DEFINITION())
	{
		m_variablesTable->TryToRegisterVariable(m_tokens);				
	}	
	else if (symbol.m_term == RuleName::EXPRESSION_AND_SYMBOL())
	{
		m_tokens;
		vector<string> stack = m_variablesTable->GetExpressionStack(m_tokens, true);				
		parseExpression(stack);	
	}	
	else if (symbol.m_term == RuleName::DEFINITION())
	{
		int size = 0;
		if (m_variablesTable->m_variablesTable ->size()) 
		{
			for (Variable var : *m_variablesTable->m_variablesTable )
			{
				size = size + var.m_size;
			}
		}		
		generator->AllocateSpace(size); //allocate space
	}
	else if (symbol.m_term == RuleName::EXPRESSION_0())
	{		
		if (m_tokens.end()[-8].lexeme == "if")
		{		
			vector<Token> expression_0;
			expression_0.push_back(m_tokens.end()[-6]);
			vector<string> ifCondition = (m_variablesTable->GetExpressionStack(expression_0, false));
			parseExpression(ifCondition);

			string falseLabel;	
			generator->WriteIfStart(falseLabel);
			createdIfExpressionsLabels.push_back(falseLabel);
		}
		else if (m_tokens.end()[-8].lexeme == "while")
		{		
			string startLabel;	
			string finLabel = generator->WriteWhileStartLabel(startLabel);
			whileLabels.push(finLabel);	
			whileLabels.push(startLabel);

			vector<Token> expression_0;
			expression_0.push_back(m_tokens.end()[-6]);
			vector<string> whileCondition = (m_variablesTable->GetExpressionStack(expression_0, false));
			parseExpression(whileCondition);
			
			generator->WriteWhileStartPart(finLabel);								
		}
	}
	else if (symbol.m_term == RuleName::GOAL()) // programm end
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
		generator->WriteProgramEnd(size);
	}
	else if (symbol.m_term == RuleName::STATEMENT()) // programm end
	{	
		if (m_tokens.at(m_tokens.size() - 4).symbol.m_term == RuleName::IDENTIFIER())
		{
			m_variablesTable->CheckExistingOfVariable(m_tokens, true);			
		}
		if (m_tokens.at(m_tokens.size() - 7).symbol.m_term == RuleName::IDENTIFIER())
		{
			m_variablesTable->CheckExistingOfVariable(m_tokens, false);
		}
		if (m_tokens.end()[-5].symbol.m_term == RuleName::WRITE())
		{
			generator->VarToConsole();
		}
		if (m_tokens.end()[-5].symbol.m_term == RuleName::READ())
		{
			generator->ConsoleToVar();
			Token var = m_tokens.end()[-3];
			if (var.nodes.size() > 1)
			{
				if (var.nodes[1].nodes.size() == 1) //одномерный массив
				{
					vector<Token> expression_0;
					expression_0.push_back(var.nodes[1].nodes.back());
					parseExpression(m_variablesTable->GetExpressionStack(expression_0, false));				
				}
				else if (var.nodes[1].nodes.size() == 3) //двумерный массив
				{
					vector<Token> expression_0, expression_1;
					expression_0.push_back(var.nodes[1].nodes.front());
					expression_1.push_back(var.nodes[1].nodes.back());	
					parseExpression(m_variablesTable->GetExpressionStack(expression_1, false));
					parseExpression(m_variablesTable->GetExpressionStack(expression_0, false));
				}
			}
			var = m_tokens.end()[-3].nodes.end()[-1];
			int offset = m_variablesTable->getOffset(var.lexeme);
			string type = m_variablesTable->getType(var.lexeme);
			int x = m_variablesTable->getXDimention(var.lexeme);			
			generator->AddWriteVarFromConsole(offset, type, x);
		}
		if (m_tokens.end()[-3].symbol.m_term == "=" && m_tokens.end()[-4].symbol.m_term != "]")
		{			
			generator->WriteAssignment(m_variablesTable->getOffset(m_tokens.end()[-4].lexeme), m_variablesTable->getType(m_tokens.end()[-4].lexeme));
		}
		if (m_tokens.end()[-3].symbol.m_term == "=" && m_tokens.end()[-4].symbol.m_term == "]")//либо одномерный, либо 2мерный массив
		{			
			Token index = m_tokens.end()[-5];
			if (index.nodes.size() == 1) //одномерный массив
			{
				vector<Token> expression_0;
				expression_0.push_back(index.nodes.back());
				parseExpression(m_variablesTable->GetExpressionStack(expression_0, false));
				generator->WriteAssignment(m_variablesTable->getOffset(m_tokens.end()[-7].lexeme), m_variablesTable->getType(m_tokens.end()[-7].lexeme));				
			}
			else if (index.nodes.size() == 3) //двумерный массив
			{
				vector<Token> expression_0, expression_1;
				expression_0.push_back(index.nodes.front());
				expression_1.push_back(index.nodes.back());	
				parseExpression(m_variablesTable->GetExpressionStack(expression_1, false));
				parseExpression(m_variablesTable->GetExpressionStack(expression_0, false));
				generator->WriteAssignment(m_variablesTable->getOffset(m_tokens.end()[-7].lexeme), 
												     m_variablesTable->getType(m_tokens.end()[-7].lexeme),
													 m_variablesTable->getXDimention(m_tokens.end()[-7].lexeme));				
			}
		}		
		if (m_tokens.end()[-7].symbol.m_term == RuleName::WHILE())
		{
			string l1, l2;
			l1 = whileLabels.top();
			whileLabels.pop();
			l2 = whileLabels.top();
			whileLabels.pop();
			generator->WriteWhileEndPart(l1, l2);
		}		
	}
	else if (symbol.m_term == RuleName::IF_CONSTRUCTION()) {
		m_tokens;
		if (!createdIfExpressionsLabels.empty()) {
			generator->WriteIfEnd(createdIfExpressionsLabels.back());
			createdIfExpressionsLabels.pop_back();
		}
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
			vector<Token> nodes;
			for (vector<int>::reverse_iterator rit = production->handles.rbegin(); rit != production->handles.rend(); rit++) {
				int symbolIndex = (*rit);				
				if (symbolIndex == m_tokens.back().symbol.m_index) {
					nodes.push_back(m_tokens.back());
					m_tokens.pop_back();
					m_states.pop_back();
				}
			}
			// Adding left part of the production to stack
			Symbol symbolNonTerminal = symbolWithIndex(production->nonTerminalIndex);

			int lineNum = 0;
			for (vector<Token>::iterator it3 = nodes.begin(); it3 != nodes.end(); it3++) 
			{
				int tokenline = it3->line;
				if (tokenline) 
				{
					lineNum = tokenline;
				}
			}

			Token nonTerminalToken(symbolNonTerminal, RuleName::NONTERMINAL(), lineNum);
			nonTerminalToken.nodes = nodes;
			m_tokens.push_back(nonTerminalToken);
			// Finding next state
			for (vector<Action>::iterator it4 = m_actionsTable->begin(); it4 != m_actionsTable->end(); it4++) 
			{
				Action *currentAction = &(*it4);
				if (currentAction->stateIndex == m_states.back() && currentAction->symbolIndex == m_tokens.back().symbol.m_index && currentAction->type == 3) 
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
			expectedValues += symbolWithIndex(action.symbolIndex).m_term;
			expectedValues += "'\n";
		}
	}
	ErrorHandler::FailedWithTokenError(expectedValues, token.lexeme, token.line);
}
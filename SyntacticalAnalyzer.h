/********************************************************************************/
/* Project: Project 2									*/
/* Author: 			Ben Levinsky						*/
/* Date: 	11/14/2016			*/
/* Description:	implement syntactic analysis on subset of scheme	*/
/********************************************************************************/
#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include <set>
#include "LexicalAnalyzer.h"
#include "CodeGen.cpp"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	token_type token;
	int Program ();
	int Stmt ();
	int StmtList ();
	int Literal ();
	int Quoted_Literal();
	int Action();
	int Param_List();
	int Else_part();

	CodeGen * generator;

	bool inFunction; // determine if current sentence is within function
	bool quotedLitFlag; // is current literal a quoted literal
	bool ifConditionFlag; // is current statement the condition in an if?
	int parenCount; // +1 for every open paren, -1 for each close paren, 0 if at top level statement. keep track of paren depth
	int stmtListFlag; // if sequence of stmt's
	string operation; // hold current op 
	bool ifStmtFlag; // is current statenment an if statement?
	
	set<token_type> ProgramFirstSet;
	set<token_type> LiteralFirstSet;
	set<token_type> QuotedLiteralFirstSet ;
	set<token_type>ActionFirstSet;
	set<token_type> StatementListFirstSet;
	set<token_type> ParamListFirstSet;
	set<token_type> ElsePartFirstSet;
	set<token_type> StatementFirstSet;


	set<token_type> StatmentFollowSet ;
	set<token_type> LiteralFollowSet ;
	set<token_type> QuotedLiteralFollowSet;
	set<token_type> StatementListFollowSet ;
	set<token_type> ParamListFollowSet ;
	set<token_type> ElsePartFollowSet ;
	set<token_type> ActionFollowSet ;


};
	
#endif

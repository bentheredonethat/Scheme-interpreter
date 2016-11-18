/********************************************************************************/
/* Project: Project 2									*/
/* Author: 			Ben Levinsky						*/
/* Date: 	11/14/2016			*/
/* Description:	implement syntactic analysis on subset of scheme	*/
/********************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
/********************************************************************************/
/* This function will	construct lexer and error count							*/
/********************************************************************************/
	lex = new LexicalAnalyzer (filename);
	token = lex->GetToken();
	ProgramFirstSet = {LPAREN_T, SYMBOL_T, QUOTE_T , NUMLIT_T};
	LiteralFirstSet = {NUMLIT_T, SYMBOL_T, QUOTE_T };
	QuotedLiteralFirstSet = {NUMLIT_T, SYMBOL_T, LPAREN_T };
	ActionFirstSet	= { SYMBOLP_T, NUMBERP_T, LISTOP_T, DEFINE_T ,SYMBOL_T,	IF_T , CAR_T, CDR_T, CONS_T , AND_T ,	OR_T ,	NOT_T ,	LISTP_T ,	ZEROP_T ,	NULLP_T ,	CHARP_T ,	STRINGP_T ,	PLUS_T ,	MINUS_T ,	DIV_T ,	MULT_T ,EQUALTO_T ,GT_T ,	LT_T ,GTE_T ,LTE_T };
	StatementListFirstSet = ProgramFirstSet;
	ParamListFirstSet = {SYMBOL_T };
	ElsePartFirstSet = ProgramFirstSet;
	StatementFirstSet = ProgramFirstSet;


	StatmentFollowSet = {RPAREN_T, EOF_T, LPAREN_T, NUMLIT_T, SYMBOL_T , QUOTE_T };
	LiteralFollowSet = StatmentFollowSet;
	QuotedLiteralFollowSet = {RPAREN_T, EOF_T, LPAREN_T, NUMLIT_T, SYMBOL_T , QUOTE_T };
	StatementListFollowSet = {RPAREN_T , EOF_T};
	ParamListFollowSet = {RPAREN_T };
	ElsePartFollowSet = ParamListFollowSet;
	ActionFollowSet = ParamListFollowSet;


	int errors = Program ();

}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
/********************************************************************************/
/* This function will	deallocate lexer							*/
/********************************************************************************/
	delete lex;
}

int SyntacticalAnalyzer::Program ()
{
/********************************************************************************/
/* This function will	evaluate if sequence of tokens conforms to program syntax							*/
/********************************************************************************/
	lex->debug << "Program function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;
	// token should be in the firsts of program
	// if true... keep going

	// if false...
	if (ProgramFirstSet.count(token) == 0){
			lex->ReportError ("err current character should be : in firsts of program");
			errors++;
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the firsts of program

	errors += Stmt ();
	// lex->debug <<"back in program after a call to stmt()\n";
	errors += StmtList ();

	if (token != EOF_T){
		lex->ReportError ("Expected end of file; " + lex->GetLexeme ());
		errors++;
	}

	lex->debug << "Program function returning " << errors << " errors\n";
	return errors;
}



int SyntacticalAnalyzer::Stmt ()
{
/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to statement syntax								*/
/********************************************************************************/
	lex->debug << "Stmt function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;

	// token should be in the firsts of stmt
	// if true... keep going

	// if false...
	if (StatementFirstSet.count(token) == 0){
			lex->ReportError ("err in first set of stmt");
			errors++;
			token = lex->GetToken();
	}
	// if false...
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the firsts of stmt

	if (token == LPAREN_T){
		// rule 5
		token = lex->GetToken();
		errors += Action();
		if (token == RPAREN_T){
			// now go to follow set token
			// lex->debug << "checking if '" << lex->GetLexeme() << "' is " << "')'\n";
			token = lex->GetToken();		
		}else{
			lex->ReportError ("Expected ')'; got " + lex->GetLexeme ());
			errors++;
		}
	}
	else{
		// rule 4
		errors += Literal();
		
	}

	

	// token should be in the follows of stmt
	// if true... keep going
	// if false...
	// lex->debug << "about to check current token in stmt follow set; current token is " << lex->GetTokenName(token) << endl;
	if (StatmentFollowSet.count(token) == 0){
		lex->ReportError ("err current character is not in follows of statement");
		errors++;
		token = lex->GetToken();
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << "statement function returning " << errors << " errors\n";

	return errors;
}
	

int SyntacticalAnalyzer::StmtList ()
{
/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to statement list syntax								*/
/********************************************************************************/
	lex->debug << "Stmt list function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;

	// check if current token corresponds to stmt, if not return
	if (StatementFirstSet.count(token) != 0){
		errors += Stmt ();
		if (StatmentFollowSet.count(token)){
			errors += StmtList ();
		}
	}
	else{
		lex->debug << "in stmt list; current expression is empty\n";
	}
	
	// token should be in the follows of stmt
	// if true... keep going
	// if false...
	// lex->debug << "about to check current token in StmtList follow set; current token is " << lex->GetTokenName(token) << endl;
	if (StatementListFollowSet.count(token) == 0){
		lex->ReportError ("err in follows of statement list");
		errors++;
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << "statement list function returning " << errors << " errors\n";

	return errors;
}

int SyntacticalAnalyzer::Quoted_Literal ()
{
/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to literal syntax								*/
/********************************************************************************/
	lex->debug << "quoted literal function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;

	// token should be in the firsts of stmt
	// if true... keep going

	// if false...
	if (QuotedLiteralFirstSet.count(token) == 0){
			lex->ReportError ("err in first set of literal");
			errors++;
			token = lex->GetToken();
	}
	// if false...
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the firsts of stmt
	

	// rule 11
	if (token == LPAREN_T){
		token = lex->GetToken();
		errors += StmtList();
		
		if (token != RPAREN_T){
			lex->ReportError("Quoted_Literal: expected ')'' after end of statment list");
			errors++;
		}
		token = lex->GetToken(); // go to token in follow set
	}

	// rule 9 and 10
	else if(token == SYMBOL_T || token == NUMLIT_T){
		token = lex->GetToken(); // go to token in follow set
	}
	else{
		lex->ReportError("Quoted_Literal: expected symbol, number literal, or statement list");
		errors++;
	}

	// token should be in the follows of stmt
	// if true... keep going
	// if false...
	// lex->debug << "about to check current token in Quoted_Literal follow set; current token is " << lex->GetTokenName(token) << endl;
	if (QuotedLiteralFollowSet.count(token) == 0){
		lex->ReportError ("err in follows of quoted literal");
		errors++;
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << "quoted literal function returning " << errors << " errors\n";

	return errors;
}


int SyntacticalAnalyzer::Literal ()
{
/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to literal syntax								*/
/********************************************************************************/
	lex->debug << "literal function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;

	// token should be in the firsts of stmt
	// if true... keep going
	// if false...
	if (LiteralFirstSet.count(token) == 0){
			lex->ReportError ("err in first set of literal");
			errors++;
			token = lex->GetToken();
	}
	// if false...
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the firsts of stmt	
	if (token == QUOTE_T){
		// rule 8
		token = lex->GetToken();
		errors += Quoted_Literal();	
	}
	else if (token == NUMLIT_T || token == SYMBOL_T)
	{ // Rule 6 and 7
		token = lex->GetToken();
	}
	else{
		lex->ReportError ("err in literal: expected number literal, symbol or quoted literal; got" + lex->GetTokenName(token));
		errors++;
	}

	// token should be in the follows of stmt
	// if true... keep going
	// if false...
	// lex->debug << "about to check current token in Literal follow set; current token is " << lex->GetTokenName(token) << endl;
	if (LiteralFollowSet.count(token) == 0){
		lex->ReportError ("err in follows of literal");
		errors++;
		token = lex->GetToken();
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << "literal function returning " << errors << " errors\n";

	return errors;
}


int SyntacticalAnalyzer::Action(){
/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to action syntax								*/
/********************************************************************************/
	lex->debug << "action function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;

	// token should be in the firsts of stmt
	// if true... keep going

	// if false...
	if (ActionFirstSet.count(token) == 0){
			lex->ReportError ("err not in first set of action");
			errors++;
			token = lex->GetToken();
	}

	// rule 12
	if (token == DEFINE_T){
		
		// define syntax: (define symbol literal)
		token = lex->GetToken();
		token_type nextToken = lex->GetToken();
		 if ( token == LPAREN_T && nextToken == SYMBOL_T){
		 	token = lex->GetToken();
			lex->debug << "in the define ( symbol ) stmt() syntax" << endl;
			errors += Param_List();
			if (token == RPAREN_T){
				token = lex->GetToken(); // set up token for statment call
				errors += Stmt();
			}
			else{
				lex->ReportError("expected ')' after Param_List '; got " + lex->GetLexeme());
				errors++;
				token = lex->GetToken(); // set up token for statment call
			}
		}else{
				lex->ReportError("expected '(' followed by symbol after define; got "
				 + lex->GetTokenName(token) + " followed by " + lex->GetTokenName(nextToken) );
				errors++;
				token = lex->GetToken(); // set up token for statment call
			}
		
		
	}
	// rule 15
	else if (token == IF_T){
		// if syntax: if stmt() stmt() else()
		lex->debug << "before stmt, stmt Else_part num errors: " << errors << endl;
		token = lex->GetToken();
		errors += Stmt();
		lex->debug << "before second stmt, in if stmt num errors: " << errors << endl;
		errors += Stmt();
		lex->debug << "after stmt, stmt but before Else_part num errors: " << errors << endl;
		errors += Else_part();
		lex->debug << "after stmt, stmt Else_part num errors: " << errors << endl;
	}

	else {
		
		switch (token){
			// stmt
			case CAR_T: case CDR_T: case NOT_T: case LISTOP_T: case NUMBERP_T: 
			case SYMBOLP_T: case LISTP_T: case ZEROP_T: case NULLP_T: 
			case CHARP_T: case STRINGP_T: 
				
				token = lex->GetToken();
				errors += Stmt();
				lex->debug << "current token is in action rule stmt " << lex->GetTokenName(token) << endl;
				break;

			// stmt_list
			case AND_T: case OR_T: case PLUS_T: case MULT_T: case EQUALTO_T: case GT_T:
			case GTE_T: case LT_T: case LTE_T: case SYMBOL_T:
				token = lex->GetToken();
				errors += StmtList();
				lex->debug << "current token is in action rule stmtlist " << lex->GetTokenName(token) << endl;
				break;
			
			// stmt stmt
			case CONS_T:
				
				token = lex->GetToken();
				errors += Stmt();
				lex->debug << "after first stmt in CONS there are " << errors << " errors " << endl;
				errors += Stmt();
				lex->debug << "after second stmt in CONS there are " << errors << " errors " << endl;
				lex->debug << "current token is in action rule stmt smt " << lex->GetTokenName(token) << endl;
				break;

			// stmt stmt_list
			case MINUS_T: case DIV_T:
				token = lex->GetToken();
				errors += Stmt();
				errors += StmtList();
				lex->debug << "current token is in action rule stmt stmtlist " << lex->GetTokenName(token) << endl;
				break;

			default:
				
				errors++;
				lex->ReportError("current token does not correspond to action syntax; " );
				break;

			}
	}

	// token should be in the follows of stmt
	// if true... keep going

	// if false...
	lex->debug << "about to check current token in action follow set; current token is " << lex->GetTokenName(token) << endl;
	if (ActionFollowSet.count(token) == 0){
		lex->ReportError ("err in follows of action");
		errors++;
	}else{
		lex->debug << lex->GetTokenName(token) << " is in action follow set" << endl;
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << "action function returning " << errors << " errors\n";

	return errors;
}

int SyntacticalAnalyzer::Param_List(){
	/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to param list syntax								*/
/********************************************************************************/
	lex->debug << "Param_List function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;

	// if already at end of parameter list, apply lambda rule
	if ( ParamListFollowSet.count(token)  ){
		lex->debug << "applying lambda rule in Param_List" << endl;
		return 0;	
	} 


	// otherwise check similar to other rules
	if ( ParamListFirstSet.count(token) == 0 ){
			lex->ReportError ("err not in first set of param list");
			errors++;

	}
	token = lex->GetToken();


	

	// check if 13 applies 
	if (token == SYMBOL_T){
		errors += Param_List();
	}
	//otherwise use rule 14: lambda rule

	// token should be in the follows of stmt
	// if true... keep going
	// if false...
	// lex->debug << "about to check current token in Param_List follow set; current token is " << lex->GetTokenName(token) << endl;
	if (ParamListFollowSet.count(token) == 0 ){
		lex->ReportError ("err in follows of param list");
		errors++;
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << "param list function returning " << errors << " errors\n";
	return errors;
}

int SyntacticalAnalyzer::Else_part(){
	/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to else part syntax								*/
/********************************************************************************/

	int errors = 0;
	lex->debug << "Else_part function called; current token is " << lex->GetTokenName(token) << endl;
// check if current token corresponds to stmt, if not return
	if (StatementFirstSet.count(token) != 0){
		errors += Stmt ();
		if (StatmentFollowSet.count(token)){
		}
	}
	else{
		lex->debug << "in Else_part; current expression is empty\n";
	}


	// token should be in the follows of stmt
	// if true... keep going
	// if false...
	// lex->debug << "about to check current token in Else_part follow set; current token is " << lex->GetTokenName(token) << endl;
	if ( ElsePartFollowSet.count(token) == 0 ){
		lex->ReportError ("err in follows of else part ");
		errors++;
	}
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of stmt
	lex->debug << " else part function returning " << errors << " errors\n";
	return errors;
}




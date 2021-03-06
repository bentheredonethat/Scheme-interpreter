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
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
/********************************************************************************/
/* This function will	construct lexer and error count							*/
/********************************************************************************/
	lex = new LexicalAnalyzer (filename);
	token = lex->GetToken();
	
	generator = new CodeGen(filename);

	inFunction = false;
	parenCount = 0;
	stmtListFlag = 3;
	ifConditionFlag = false;
	quotedLitFlag = false;
	ifStmtFlag = false;

	operation = "";

	// initialize first and follow sets of non-terminals
	ProgramFirstSet = {LPAREN_T, SYMBOL_T, QUOTE_T , NUMLIT_T};
	LiteralFirstSet = {NUMLIT_T, SYMBOL_T, QUOTE_T };
	QuotedLiteralFirstSet = {NUMLIT_T, SYMBOL_T, LPAREN_T };
	ActionFirstSet	= { SYMBOLP_T, NUMBERP_T, LISTOP_T, DEFINE_T ,SYMBOL_T,	IF_T , CAR_T, CDR_T, CONS_T , AND_T ,	OR_T ,	NOT_T ,	LISTP_T ,	ZEROP_T ,	NULLP_T ,	CHARP_T ,	STRINGP_T ,	PLUS_T ,	MINUS_T ,	DIV_T ,	MULT_T ,EQUALTO_T ,GT_T ,	LT_T ,GTE_T ,LTE_T };
	StatementListFirstSet = {LPAREN_T , NUMLIT_T, SYMBOL_T , QUOTE_T , EOF_T , RPAREN_T };
	ParamListFirstSet = {SYMBOL_T , RPAREN_T };
	ElsePartFirstSet = {LPAREN_T, NUMLIT_T, SYMBOL_T, QUOTE_T , RPAREN_T };
	StatementFirstSet = {LPAREN_T , NUMLIT_T , SYMBOL_T, QUOTE_T };


	StatmentFollowSet = {RPAREN_T, EOF_T, LPAREN_T, NUMLIT_T, SYMBOL_T , QUOTE_T };
	LiteralFollowSet = { RPAREN_T  ,    LPAREN_T ,      NUMLIT_T   ,   SYMBOL_T    ,  QUOTE_T ,EOF_T };
	QuotedLiteralFollowSet = {RPAREN_T, EOF_T, LPAREN_T, NUMLIT_T, SYMBOL_T , QUOTE_T };
	StatementListFollowSet = {RPAREN_T , EOF_T};
	ParamListFollowSet = {RPAREN_T };
	ElsePartFollowSet = ParamListFollowSet;
	ActionFollowSet = ParamListFollowSet;

	int errors = Program ();

	cout << errors << " errors found in input file\n";
	delete generator;


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
	SetLimits();


	lex->debug << "Program function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;
	
	// check if current token is in the first set of program
	if (ProgramFirstSet.count(token) == 0){
			lex->ReportError ("expected '(LPAREN_T)', symbol, \'' , or number; before program" + lex->GetLexeme());
			errors++;
	}


	errors += Stmt ();
	errors += StmtList ();

	// if program does not end with EOF, report error
	if (token != EOF_T){
		lex->GetToken();
		lex->ReportError ("Expected end of file; after program" + lex->GetLexeme ());
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

	// check if current token is in first set of statment
	if (StatementFirstSet.count(token) == 0){
			lex->ReportError ("expected ( , number, symbol , \' , end-of-file , ) before statment; " + lex->GetLexeme());
			errors++;
			token = lex->GetToken();
	}

	// if current token is '(' then try ( action ) syntax
	// otherwise literal syntax rule
	

	if (!inFunction && !parenCount && token != LPAREN_T){
		lex->debug << "generating c++ cout \n";	
		generator->writeCout();
	}


	if (token == LPAREN_T){

		// rule 5
		token = lex->GetToken(); // walk to next token and try action syntax
		
		errors += Action();

		
		if (token == RPAREN_T){
			token = lex->GetToken();		
		}else{
			lex->ReportError ("Expected ')' after action; got " + lex->GetLexeme ());
			errors++;
		}
	}
	else{
		// rule 4
		
		errors += Literal();
		if (!parenCount && !inFunction) generator->writeEndl();
	}


	// check if current token is in follow set of statement
	if (StatmentFollowSet.count(token) == 0){
		lex->ReportError ("expected (, ), end-of-file, number literal, symbol, or ' after statement; " + lex->GetLexeme());
		errors++;
		token = lex->GetToken();
	}
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

	// check if current token is in first set of statment list
	if (StatementListFirstSet.count(token) == 0){
			lex->ReportError ("expected ( , number, symbol , \' , end-of-file , ) before statement list; " + lex->GetLexeme());
			errors++;
			token = lex->GetToken();
	}

	

	// check if current token is within the first set of stmt
	// if yes then apply stmt stmtlist syntax rule
	// if not, apply lambda rule
	if (StatementFirstSet.count(token) != 0){
		// 
		
		errors += Stmt ();
		if (StatementFirstSet.count(token)){
			// only put separator if there is something after current stmt
			generator->separator(stmtListFlag, operation);	
		}
		

		if (StatmentFollowSet.count(token)){
			errors += StmtList ();

		}
	}
	else{
		lex->debug << "in stmt list; current expression is empty\n";
	}
	// check if current token is in follow set of statement list
	if (StatementListFollowSet.count(token) == 0){
		lex->ReportError ("expected ) or end-of-file after statement list; got" + lex->GetLexeme());
		errors++;
	}
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
	// stmtListFlag = 0; // we're using spaces

	// check if current token is in first set of quoted literal
	if (QuotedLiteralFirstSet.count(token) == 0){
			lex->ReportError ("expected number, symbol or '(' before quoted literal; " + lex->GetLexeme());
			errors++;
			token = lex->GetToken();
	}

	// rule 11
	if (token == LPAREN_T){

		token = lex->GetToken();
		errors += StmtList();
		
		if (token != RPAREN_T){
			lex->ReportError("Quoted_Literal: expected ')' after end of statment list");
			errors++;
		}
		token = lex->GetToken(); // go to token in follow set
	}

	// rule 9 and 10
	else if(token == SYMBOL_T || token == NUMLIT_T){
		lex->debug << "outputing lit to quoted lit  "  ;
		generator->outputLexemeName(lex->GetLexeme());
		token = lex->GetToken(); // go to token in follow set
	}
	else{
		lex->ReportError("Quoted_Literal: expected symbol, number, or statement list");
		errors++;
	}

	// check if current token is in follow set of quoted literal
	if (QuotedLiteralFollowSet.count(token) == 0){
		lex->ReportError ("expected ), end-of-file, (, number, symbol, or ' after quoted literal; got" + lex->GetLexeme());
		errors++;
	}
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

	// check if current token is in first set of  literal
	if (LiteralFirstSet.count(token) == 0){
			lex->ReportError ("expected number, symbol or '(' before literal; " + lex->GetLexeme());
			errors++;
			token = lex->GetToken();
	}


		if (inFunction && !parenCount && !ifConditionFlag){
			generator->writeReturn();	
		} 

	if (token == QUOTE_T){
		// rule 8
		generator->beginLit();
		generator->quote();
		
		stmtListFlag = 1;
		parenCount +=1;
		lex->debug << "in quoted lit";
		token = lex->GetToken();

		bool gotList = ( token == LPAREN_T );
		if (gotList){
			generator->writeOpenParen();
		}
		quotedLitFlag = true;
		errors += Quoted_Literal();	
		quotedLitFlag = false;
		
		if (gotList){
			generator->writeCloseParen();
		}


		generator->quote();
		stmtListFlag = 3;
		parenCount -=1;
		generator->writeCloseParen();
	}
	else if (token == NUMLIT_T)
	{ // Rule 6 and 7
		if (!quotedLitFlag) generator->beginLit();
		generator->outputLexemeName(lex->GetLexeme());
		token = lex->GetToken();
		if (!quotedLitFlag) generator->writeCloseParen();
	}

	else if (token == SYMBOL_T)
	{ // Rule 6 and 7
		generator->outputLexemeName(lex->GetLexeme());
		token = lex->GetToken();
	}



	else{
		lex->ReportError ("err in literal: expected number literal, symbol or quoted literal; got" + lex->GetTokenName(token));
		errors++;
	}
	if (inFunction && !parenCount && !ifConditionFlag){
		generator->writeSemicolon();	
	} 
	// check if current token is in follow set of  literal
	if (LiteralFollowSet.count(token) == 0){
		lex->ReportError ("expected ), end-of-file, (, number, symbol, or ' after literal; got " + lex->GetLexeme());
		errors++;
	}
	lex->debug << "literal function returning " << errors << " errors\n";

	return errors;
}


int SyntacticalAnalyzer::Action(){
/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to action syntax								*/
/********************************************************************************/
	lex->debug << "action function called; current token is " << lex->GetTokenName(token) << endl;
	int errors = 0;
	// check if current token is in first set of  action
	if (ActionFirstSet.count(token) == 0){
			lex->ReportError ("current token should not precede action; " + lex->GetLexeme());
			errors++;
			token = lex->GetToken();
	}

	// rule 12
	if (token == DEFINE_T){
		inFunction = true;
		generator->setFunctionFlag(inFunction);

		token = lex->GetToken();

		token_type nextToken = lex->GetToken();
				generator->fnheader(lex->GetLexeme());

		 if ( token == LPAREN_T && nextToken == SYMBOL_T){
		 	
		 	token = lex->GetToken();
			errors += Param_List();
			generator->closefnheader();
			if (token == RPAREN_T){
				token = lex->GetToken(); // set up token for statment call

				errors += Stmt();
				generator->closeFnImpl();
				inFunction = false;	
				generator->setFunctionFlag(inFunction);
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
	else if (token == IF_T){ // if syntax: if stmt() stmt() else()
		ifStmtFlag = true;
		token = lex->GetToken();
		
		parenCount += 1;
		generator->startIf();
		ifConditionFlag = true;
		errors += Stmt();
		ifConditionFlag = false;
		generator->closeIf();
		parenCount -= 1;
		errors += Stmt();
		parenCount += 1;
		generator->closeElse();
		parenCount -= 1;
		errors += Else_part();

	}

	else {
		if (!inFunction && !parenCount && token != LPAREN_T){
			lex->debug << "generating c++ cout \n";	
			generator->writeCout();
		}
		if (inFunction && !parenCount && !ifConditionFlag){
			generator->writeReturn();	
		} 
		generator->writeOpenParen();
		parenCount++;

		string actionName;
		string str;
		switch (token){
			lex->debug << "generating action";
			// stmt
			case CAR_T: case CDR_T: case NOT_T: case LISTOP_T: case NUMBERP_T: 
			case SYMBOLP_T: case LISTP_T: case ZEROP_T: case NULLP_T: 
			case CHARP_T: case STRINGP_T: 
				str = (token == NOT_T) ? "!" : lex->GetLexeme();
				// if predicate, replace ? with p so that Object class can use built in function
				if (str[str.size()-1] == '?'){
					str.erase (str.size()-1);
					str += 'p';

				}
				generator->outputLexemeName(str);
				generator->writeOpenParen();
				parenCount+=1;
				token = lex->GetToken();
				errors += Stmt();
				generator->writeCloseParen();
				parenCount-=1;
				break;

			// stmt_list
			case AND_T: case OR_T: case PLUS_T: case MULT_T: case EQUALTO_T: case GT_T:
			case GTE_T: case LT_T: case LTE_T: case SYMBOL_T:
				if (token != SYMBOL_T){
					
					operation = lex->GetLexeme();
					if (token == AND_T) operation = "&&";
					if (token == OR_T) operation = "||";
					if (token == EQUALTO_T) operation = "==";
					
					token = lex->GetToken();
					// if empty then done
					if (token == RPAREN_T){
						generator->outputLexemeName(operation);
						// do nothing
					}
					else {
						// number literals have to be wrapped in object lit
						if (token == NUMLIT_T) generator->beginLit();
						generator->outputLexemeName(lex->GetLexeme());
						if (token == NUMLIT_T) generator->writeCloseParen();

						stmtListFlag = 2;
						generator->separator(stmtListFlag, operation);
						
						token = lex->GetToken();
						errors += StmtList();	
							
					}
					stmtListFlag = 3;
					
				}
				else{
					int oldFlag = stmtListFlag;
					 parenCount += 1;
					 generator->outputLexemeName(lex->GetLexeme());
					 if (!quotedLitFlag) generator->writeOpenParen();

					token = lex->GetToken();
					stmtListFlag = 0;
					errors += StmtList();	
					if (!quotedLitFlag)  generator->writeCloseParen();
					parenCount -=1;
					stmtListFlag = oldFlag;

				}
				
				
				break;
			
			// stmt stmt
			case CONS_T:
				parenCount += 1;
				generator->outputLexemeName(lex->GetLexeme());
				generator->writeOpenParen();
				
				token = lex->GetToken();
				errors += Stmt();
				stmtListFlag = 0;
				generator->separator(stmtListFlag);
				errors += Stmt();
				generator->writeCloseParen();
				parenCount -=1;
				break;

			// stmt stmt_list
			case MINUS_T: case DIV_T:
				operation = lex->GetLexeme();
				token = lex->GetToken();
				// number literals have to be wrapped in object lit
				if (token == NUMLIT_T) generator->beginLit();
				generator->outputLexemeName(lex->GetLexeme());
				if (token == NUMLIT_T) generator->writeCloseParen();

				stmtListFlag = 2; // use operator
				generator->separator(stmtListFlag, operation);
				
				token = lex->GetToken();
				errors += StmtList();	
				stmtListFlag = 3;
				break;

			default:
				
				errors++;
				lex->ReportError("current token does not correspond to action syntax; " );
				break;

			}
	
		generator->writeCloseParen();
		parenCount--;
		if (!parenCount && !inFunction) generator->writeEndl();

		else if (inFunction && !parenCount && !ifConditionFlag){
			generator->writeSemicolon();	
		}
		
	}

	if (ActionFollowSet.count(token) == 0){
		lex->ReportError ("current token is not expected after action; " + lex->GetLexeme());
		errors++;
	}
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
		// 
		lex->debug << "applying lambda rule in Param_List" << endl;
		return 0;	
	} 
	

	// otherwise check similar to other rules
	if ( ParamListFirstSet.count(token) == 0 ){
			lex->ReportError ("err not in first set of param list");
			errors++;

	}
	generator->outputFnParam(lex->GetLexeme());
	token = lex->GetToken();

	// check if 13 applies 
	if (token == SYMBOL_T){
		generator->separator(0);
		
		errors += Param_List();
	}
	//otherwise use rule 14: lambda rule
	if (ParamListFollowSet.count(token) == 0 ){
		lex->ReportError ("current token should not be after parameter list; " + lex->GetLexeme());
		errors++;
	}
	lex->debug << "param list function returning " << errors << " errors\n";
	return errors;
}

int SyntacticalAnalyzer::Else_part(){
	/********************************************************************************/
/* This function will evaluate if sequence of tokens conforms to else part syntax								*/
/********************************************************************************/

	int errors = 0;
	lex->debug << "Else_part function called; current token is " << lex->GetTokenName(token) << endl;

	if ( ElsePartFirstSet.count(token) == 0 ){
		lex->ReportError ("current token is not expected before else part; " + lex->GetLexeme());
		errors++;
	}

	// apply else part --> stmt() rule only if current token is in first set of stmt
	// otherwise apply lambda rule
	if (StatementFirstSet.count(token) != 0){
		generator->beginElse();
		errors += Stmt ();
		generator->closeElse();
		
	}

	if ( ElsePartFollowSet.count(token) == 0 ){
		lex->ReportError ("current token is not expected after else part; " + lex->GetLexeme());
		errors++;
	}
	lex->debug << " else part function returning " << errors << " errors\n";
	return errors;
}




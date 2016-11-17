/********************************************************************************/
/* Project: 									*/
/* Author: 									*/
/* Date: 									*/
/* Description:									*/
/********************************************************************************/

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum token_type {NONE = -1,SYMBOL_T, NUMLIT_T, CAR_T, CDR_T, CONS_T, IF_T,
		LISTOP_T, AND_T, OR_T, NOT_T, DEFINE_T, NUMBERP_T, SYMBOLP_T,
		LISTP_T, ZEROP_T, NULLP_T, CHARP_T, STRINGP_T, PLUS_T,
		MINUS_T, DIV_T, MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T,
		LTE_T, LPAREN_T, RPAREN_T, QUOTE_T, ERROR_T, EOF_T};

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);
	ofstream debug;
    private:
	ifstream input;
	ofstream listing;
	string line;
	int linenum;
	int pos;
	string lexeme;
	int errors;
};
	
#endif

#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"SYMBOL_T", "NUMLIT_T", "CAR_T", "CDR_T", "CONS_T", "IF_T",
				"LISTOP_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T", "NUMBERP_T",
				"SYMBOLP_T", "LISTP_T", "ZEROP_T", "NULLP_T", "CHARP_T",
				"STRINGP_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T",
				"GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T",
				"QUOTE_T", "ERROR_T", "EOF_T"}; 
static int table [][19] = 
//	  a   c   d   r   b   0   .   (   )   +   -   *   /   '   =   <   >   ?  other
       {{32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},  // 0 not used
	{ 8,  9,  8,  8,  8,  2,  3, 24, 24,  5,  6, 24, 24, 24, 24,  7,  7, 32, 24},  // 1 starting
	{22, 22, 22, 22, 22,  2,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 2 number
	{31, 31, 31, 31, 31,  4, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},  // 3 period
	{22, 22, 22, 22, 22,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 4 number after decimal
	{23, 23, 23, 23, 23,  2,  3, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},  // 5 plus
	{23, 23, 23, 23, 23,  2,  3, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},  // 6 minus
	{23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 23, 23, 23, 23},  // 7 less than
	{ 8,  8,  8,  8,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 24, 21},  // 8 letter other than c
	{10,  8, 11,  8,  8,  8, 21, 21, 21, 24, 21, 10, 21, 21, 21, 21, 21, 21, 21},  // 9 letter c
	{ 8,  8, 11, 20,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},  // 10 letter a after c
	{ 8,  8, 11, 20,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21}}; // 11 letter d after c, ca, or cd...

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	cout << "Input file: " << filename << endl;
	string name = filename;
	string extension;
	if (name.length() > 3)
		extension = name.substr (name.length()-3, 3);
	if (extension != ".ss")
	{
		cout << "Invalid file extension; must be '.ss'\n";
		exit (1);
	}
	input.open (filename);
	if (input.fail())
	{
		cout << "File " << filename << " not found\n";
		exit (2);
	}
	string listingname = name.substr (0, name.length()-3) + ".lst"; 
	listing.open (listingname.c_str());
	listing << "Input file: " << filename << endl;
	string debugname = name.substr (0, name.length()-3) + ".dbg"; 
	debug.open (debugname.c_str());
	debug << "Input file: " << filename << endl;
	line = " ";
	linenum = 0;
	pos = 0;
	lexeme = "";
	errors = 0;
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	input.close ();
	cout << errors << " errors found in input file\n";
	listing << errors << " errors found in input file\n";
	listing.close();
	debug << errors << " errors found in input file\n";
	debug.close();
}

token_type LexicalAnalyzer::GetToken ()
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	static string valid = "acdrb0.()+-*/'=<>?";
	while (isspace(line[pos]))
		if (++pos >= line.length())
		{
			getline (input, line);
			if (input.fail())
				return EOF_T;
			linenum++;
			listing << setw(4) << right << linenum << ": " << line << endl;	
			debug << setw(4) << right << linenum << ": " << line << endl;	
			line += ' ';
			pos = 0;
		}
	lexeme = "";
	int state = 1;
	token_type token = NONE;
	while (token == NONE)
	{
		char c = line[pos++];
		lexeme += c;
		if (isalpha(c) && (c != 'a' && c != 'c' && c != 'd' && c != 'r')) 
			c = 'b';
		else if (isdigit(c))
			c = '0';
		int col = 0;
		while (col < valid.length() && valid[col] != c)
			col++;
		state = table[state][col];
		switch (state)
		{
		    case 20: 
			if (lexeme == "car") token = CAR_T;
			else if (lexeme == "cdr") token = CDR_T;
			else token = LISTOP_T;
			break;
		    case 21: 
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			if (lexeme == "cons") token = CONS_T;
			else if (lexeme == "if") token = IF_T;
			else if (lexeme == "and") token = AND_T;
			else if (lexeme == "or") token = OR_T;
			else if (lexeme == "not") token = NOT_T;
			else if (lexeme == "define") token = DEFINE_T;
			else token = SYMBOL_T;
			break;
		    case 22: 
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			token = NUMLIT_T;
			break;
		    case 23: 
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 24: 
			if (lexeme == "number?") token = NUMBERP_T;
			else if (lexeme == "symbol?") token = SYMBOLP_T;
			else if (lexeme == "list?") token = LISTP_T;
			else if (lexeme == "zero?") token = ZEROP_T;
			else if (lexeme == "null?") token = NULLP_T;
			else if (lexeme == "char?") token = CHARP_T;
			else if (lexeme == "string?") token = STRINGP_T;
			else if (lexeme == "+") token = PLUS_T;
			else if (lexeme == "-") token = MINUS_T;
			else if (lexeme == "/") token = DIV_T;
			else if (lexeme == "*") token = MULT_T;
			else if (lexeme == "=") token = EQUALTO_T;
			else if (lexeme == "<") token = LT_T;
			else if (lexeme == ">") token = GT_T;
			else if (lexeme == "(") token = LPAREN_T;
			else if (lexeme == ")") token = RPAREN_T;
			else if (lexeme == "'") token = QUOTE_T;
			else if (lexeme == "<=") token = LTE_T;
			else if (lexeme == ">=") token = GTE_T;
			else token = ERROR_T;
			break;
		    case 31: 
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 32: 
			
			
			token = ERROR_T;
		}
		if (token == ERROR_T)
		{
			ReportError (string("Invalid character found: ") + lexeme);
			errors++;
		}
	}
	debug << '\t' << setw(16) << left << token_names[token] << lexeme << endl;
	return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	return token_names [t];
}

string LexicalAnalyzer::GetLexeme () const
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	listing << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	debug << "Error at " << linenum << ',' << pos << ": " << msg << endl;
}

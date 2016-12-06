#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;
class CodeGen{

public:
	CodeGen(string lstFile){ // assume .ss file from syntax analyzer

		// check validity of input
		if (lstFile.size() == 0) {
			cout << "invalid listing file" << endl;
			exit(1);

		}
		if (lstFile.find("lst") == string::npos){
			cout << "invalid listing file" << endl;
			exit(1);			
		}

		currentCharacter = 0;


		// prep output file name
		string otuputFileName = lstFile;
		 // remove 'ss' from string
		otuputFileName.pop_back();
		otuputFileName.pop_back();
		otuputFileName += "cpp"; // then append ss for outputfile


		ofstream outputFile(otuputFileName);
		ifstream inputFile(lstFile);

		// error checking on input and output files
		if (!outputFile.good() || !inputFile.good()){
			cout << "either input or output file is bad" << endl;
			exit(1);
		}


		functionHeadersBuffer = "// Autogenerated Scheme to C++ Code\n";
		functionHeadersBuffer += "// File: test1.cpp\n";
		functionHeadersBuffer  += "#include <iostream>\n#include “Object.h”\nUsing namespace std;\n\n";
		functionDefinitionsBuffer = "";
		mainCodeBuffer = "int main(){\n";
	}

	// fn header gen
	// given token that signifies fn name, generate beginning of fn header
	// this is not generating param list or fn definition
	// generate Object fnname (...

	// param list gen
	// called by syntactic an. directly after fn gen is called
	// generate c++ code that is sequence of symbols for sequence of params in define fn in scheme
	// generate ....){ ....



	// end of fn block
	// at end of stmt after param list, close fn code block

	// pass flag for actions and literals to determine if in fn or not


	// stmt fn
	// determine if within function or not -- this determines if this is returns or cout'ed
	// ^^ might use 'level of depth' which can be deduced by relative paren count and whether in define


	// literal gen -- handles quoted lit too
	// enclose object(....) 

	// program gen 
	// generate beginning of c++ and after EOF, generate ending of code
	void start(){
		// constructor already opens output so we should be ok
	}

	// close gen
	void end(){
		outputFile << "\n return 0;\n}\n";
	}
	// put enclosing }

	// action gen
	// 3 cases:
	// if, define and others
	/*
		1. if
		write out if, let stmt call generate enclosed condition, then write out {
			after last stmt output closing }
		2. define <-- handled by fn gen
		3. others
			a. operators
				store operator
				if stmt list is empty skip
				otherwise while there are stmts in stmt list of operator action,
				let stmt gen handle stmts and between each output operator
			b. predicates
				enclose, then put lexeme for predicate name
			c. single stmt actions
				same as pred's
			d. cons
				// simialar but put separating comma
			e. stmt list actions
				// same as operator
			f. relational actions
				same as operator

		at end of each, enclose with r paren ')'

	*/



	~CodeGen(){
			// mainCodeBuffer += "\n\nreturn 0; \n}\n";

			// close input and output files
			inputFile.close();
			outputFile.close();
	}

	// generate c++ code for scheme statement 
	// assume either fn gen or generate() has called this function and the current character is at the beginning '(' 
	// of the corresponding stmt
	// note: stmt gen only returns strings, either fn gen or generate should know where to output corresponding stmt strings
	string statementGenerator(){
		// append to a buffer that will hold the function or operator until current character is white space or R paren
		// check if call is built-in or an operator
		// if operator, handle infix operation
		// skip ws
		// if we see R paren return the corresponding empty statement because this is a fn or operator call with no params


		// otherwise there are params;
		// loop until closing r paren which signifies end of operands for stmt 
		// if we see l paren, recursively call statement generator
		// otherwise handle lists and literals 
		// once l paren is found, the stmt is closed
		// return stmt c++ code stored in string
	}

	// once '(define' is called process the corresponding fn in scheme and generate c++ code
	void functionGenerator(){
		// skip ws after define
		// begin of param list is at '('
		// skip ws after '('
		//  store fn name in buffer

		// now construct code for collection of params for this function
		// loop through input stream until ')' is found
		// each param is separated by ws

		// now out of param list
		// now have necessary info for fn header

		// now generate code for 0 or more statements in function implementation
		
		// skip ws
		// while current character is not ')' that closes function
			// if '(' is found, call use statementGenerator to generate c++ code for the stmt
			// stmtGen will land input stream at the ')' enclosing stmt just evaluated so walk to next char

		// once this loop is done, the function definition is complete and should be enclosed


		// now that fn header and defn is constructed, output both to corresponding buffers

	}

	// given a lexeme and token, output c++ code to output file
	void generate(){
			/*
				while char's left in input stream
					if current character is '('
						consume ws
					if next lexeme is 'define' call fn gen
					otherwise walk back to '(' then call stmt gen and output result in main code buffer

				once done, output fn headers, then main, then impl's to output file
			*/
	

	}

private:
	int currentCharacter; // position in input file
	ofstream outputFile; 
	ifstream inputFile;
	string functionHeadersBuffer; // collection of fn headers
	string mainCodeBuffer; // code that will go in main
	string functionDefinitionsBuffer; // collection of fn defn's
};
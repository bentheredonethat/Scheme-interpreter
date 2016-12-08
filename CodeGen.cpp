#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;
class CodeGen{

public:
	CodeGen(string filename){ // assume .ss file from syntax analyzer

		// check validity of input
		if (filename.size() == 0) {
			cout << "invalid input file" << endl;
			exit(1);

		}
		if (filename.find("ss") == string::npos){
			cout << "invalid scheme file" << endl;
			exit(1);			
		}



		// prep output file name
		 // remove 'ss' from string
		filename.pop_back();
		filename.pop_back();
		filename += "cpp"; // then append ss for outputfile


		outputFile.open(filename);

		// error checking on input and output files
		if (!outputFile.good()){
			cout << "output file is not good" << endl;
			exit(1);
		}

		parenCount = 0;
		functionFlag = false;
		
		outputFile << "// Autogenerated Scheme to C++ Code\n";
		outputFile << "// File: test1.cpp\n";
		outputFile << "#include <iostream>\n#include “Object.h”\nUsing namespace std;\n\n";
		fnBuffer = "";
		mainBuffer = "int main(){\n";
	}

	~CodeGen(){
		outputFile << fnBuffer;
		outputFile << mainBuffer;
		outputFile << "\nreturn 0;\n}\n";
		outputFile.flush();
		outputFile.close();
	}

	void outputFnArg(string lexeme){
		(functionFlag ? fnBuffer : mainBuffer) += lexeme;
	}

	void fnheader(string lexeme){
		(functionFlag ? fnBuffer : mainBuffer) += "Object " + lexeme + "( " ;
		
	}

	void quote(){
		(functionFlag ? fnBuffer : mainBuffer) += "\"" ;
		
	}

	void closeElse(){
		(functionFlag ? fnBuffer : mainBuffer) += "\n}\n ";	
		
	}

	void writeCout(){
		(functionFlag ? fnBuffer : mainBuffer) += "cout << ";
		
	}

	void beginElse(){
		(functionFlag ? fnBuffer : mainBuffer) += "\nelse{\n";	
		
	}

	void separator(int type = 0, string operation = ""){
		string separation;
		switch(type){
			case 0:
				separation = ", ";
				break;
			case 1:
				separation = " ";
				break;
			case 2:
				separation = " " + operation + " ";
				break;
			case 3:
				separation = "";
				break;
			default:
				break;
		}
		(functionFlag ? fnBuffer : mainBuffer) += separation;	
	}

	void closeParamList(){
		(functionFlag ? fnBuffer : mainBuffer) +=  "){\n return ";
		
	}

	void outputFnParam(string lexeme){
		(functionFlag ? fnBuffer : mainBuffer) += "Object " +lexeme; 
		
	}


	void outputLexemeName(string lexeme){
		(functionFlag ? fnBuffer : mainBuffer) += lexeme;
		
	}



	void writeCloseParen(){
		(functionFlag ? fnBuffer : mainBuffer) += ')';
		
	}

	void writeSemicolon(){
		(functionFlag ? fnBuffer : mainBuffer) += ";\n\t";
		
		
	}


	void writeOpenParen(){
		(functionFlag ? fnBuffer : mainBuffer) += '(';
		
		
	}

	void closeFnImpl(){
		(functionFlag ? fnBuffer : mainBuffer) +=  "\n}\n\n";
		
		
	}

	void closefnheader(){
		
		(functionFlag ? fnBuffer : mainBuffer) += "){\nreturn ";
		
	}

	void beginLit(){
		
		(functionFlag ? fnBuffer : mainBuffer) +=  "Object(";
		
	}


	void setFunctionFlag(bool state){
		functionFlag = state;
	}

	void startIf(){
		(functionFlag ? fnBuffer : mainBuffer) +=  "if";
	}
	void closeIf(){
		(functionFlag ? fnBuffer : mainBuffer) +=  "){\n ";
	}



private:
	int parenCount; // record depth of parens
	int functionFlag;
	string fnBuffer;
	string mainBuffer;
	ofstream outputFile; 
};
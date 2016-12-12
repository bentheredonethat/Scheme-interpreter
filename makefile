P3.out : Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGen.o
	g++ -g -o P3.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project3.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

CodeGen.o : CodeGen.cpp
	g++ -g -c CodeGen.cpp -std=c++11

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp -std=c++11

clean : 
	rm *.o P3.out *.gch *.dbg *lst

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf ylnP2
	mkdir ylnP2
	cp Project2.cpp ylnP2
	cp LexicalAnalyzer.h ylnP2
	cp LexicalAnalyzer.cpp ylnP2
	cp SyntacticalAnalyzer.h ylnP2
	cp SyntacticalAnalyzer.cpp ylnP2
	cp makefile ylnP2
	cp README.txt ylnP2
	tar cfvz ylnP2.tgz ylnP2
	cp ylnP2.tgz ~tiayln/cs460drop
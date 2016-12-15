P3.out : Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGen.o
	g++ -g -o P3.out Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project3.o : Project3.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project3.cpp

CodeGen.o : CodeGen.cpp
	g++ -g -c CodeGen.cpp -std=c++11 SetLimits.o


SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp



LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp -std=c++11

clean : 
	rm *.o P3.out *.gch *.dbg *lst a.out

submit : Project3.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf ylnP3
	mkdir ylnP3
	cp Project3.cpp ylnP3
	cp LexicalAnalyzer.h ylnP3
	cp LexicalAnalyzer.cpp ylnP3
	cp SyntacticalAnalyzer.h ylnP3
	cp SyntacticalAnalyzer.cpp ylnP3
	cp makefile ylnP3
	cp README.txt ylnP3
	tar cfvz ylnP3.tgz ylnP3
	cp ylnP3.tgz ~tiayln/cs460drop
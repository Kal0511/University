all: build

build:
	flex --c++ -o scanner.cc scanner.l
	bison parser.yy -o parser.cc -Wall
	g++ scanner.cc parser.cc SemanticChecker.cpp codeGen.cpp main.cpp -o golf -Wall -Wextra

clean:
	rm -f scanner.cc
	rm -f parser.cc parser.hh location.hh
	rm -f golf
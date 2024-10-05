all: build

build:
	flex++ scanner.l
	g++ lex.yy.cc main.cpp -o golf -Wall
	rm -f lex.yy.cc

clean:
	rm -f golf
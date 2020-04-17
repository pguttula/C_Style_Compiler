cc = gcc -g
CC = g++ -g

LEX=lex
YACC=yacc

all: tinyscript

lex.yy.o: tinyscript.l 
	$(LEX) tinyscript.l
	$(CC) -c lex.yy.c

y.tab.o: tinyscript.y
	$(YACC) -v -d tinyscript.y
	$(CC) -c y.tab.c

main.o: main.cc
	$(CC) -c main.cc

tinyscript: y.tab.o lex.yy.o main.o
	$(CC) lex.yy.o y.tab.o main.o -o hw3 -lfl

clean:
	rm -f hw3 lex.yy.c y.tab.c y.tab.h y.output *.o

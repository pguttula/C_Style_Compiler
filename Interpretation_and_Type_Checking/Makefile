cc = gcc -g
CC = g++ -g -Wformat-security

LEX=lex
YACC=yacc

all: tiny

lex.yy.o: tiny.l 
	$(LEX) tiny.l
	$(CC) -c lex.yy.c

y.tab.o: tiny.y
	$(YACC) -v -d tiny.y
	$(CC) -c y.tab.c

ast.o: ast.c
	$(CC) -c ast.c

typecheck.o: typecheck.c
	$(CC) -c typecheck.c

interp.o: interp.c
	$(CC) -c interp.c

main.o: main.cc
	$(CC) -c main.cc

tiny: ast.o y.tab.o lex.yy.o typecheck.o interp.o main.o
	$(CC) ast.o lex.yy.o y.tab.o typecheck.o interp.o main.o -o hw6 -lfl

clean:
	rm -f hw5 lex.yy.c y.tab.c y.tab.h y.output 1 *.o

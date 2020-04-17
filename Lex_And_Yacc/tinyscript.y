%{

#include <stdio.h>

void yyerror(const char * s);
int yylex();

%}

%token<int_val> TOKEN_INT
%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_COMPARE
%token TOKEN_LESS
%token TOKEN_MUL
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_PRINT
%token TOKEN_PRINTLN
%token TOKEN_FOR
%token TOKEN_IN
%token TOKEN_SCOLON
%token TOKEN_COMMA
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LSQUARE
%token TOKEN_RSQUARE
%token TOKEN_SUM
%token TOKEN_EQ
%token TOKEN_ID
%token TOKEN_STRING
%token TOKEN_ERROR
%union {
		int	int_val;
    char *str;
    char *id_val;
	}

%%

program:
   statments
;
statments:
   stat TOKEN_SCOLON
   |
   stat TOKEN_SCOLON statments
stat:
   TOKEN_ID TOKEN_EQ expression
   |
   TOKEN_PRINT TOKEN_LPAREN params TOKEN_RPAREN
   |
   TOKEN_PRINTLN TOKEN_LPAREN params TOKEN_RPAREN

;
atom:
   TOKEN_STRING
   |
   TOKEN_ID
   |
   TOKEN_INT
   |
   TOKEN_LPAREN expression TOKEN_RPAREN
   |
    TOKEN_FOR TOKEN_ID TOKEN_IN TOKEN_LSQUARE TOKEN_INT TOKEN_COMMA TOKEN_INT TOKEN_RSQUARE TOKEN_SUM TOKEN_LPAREN expression TOKEN_RPAREN
;
expression:
    TOKEN_ID
    |
    TOKEN_INT
    |
    TOKEN_STRING
    |
    TOKEN_FOR TOKEN_ID TOKEN_IN TOKEN_LSQUARE TOKEN_INT TOKEN_COMMA TOKEN_INT TOKEN_RSQUARE TOKEN_SUM TOKEN_LPAREN expression TOKEN_RPAREN
    |
    expression TOKEN_ADD atom
    |
    expression TOKEN_SUB atom
    |
    expression TOKEN_LESS atom
    |
    expression TOKEN_COMPARE atom
    |
    expression TOKEN_AND atom
    |
    expression TOKEN_OR atom
    |
    TOKEN_LPAREN  expression  TOKEN_RPAREN
;
params:
    param 
    |
    param TOKEN_COMMA params
;
param:
    expression
;



%%

void
yyerror(const char * s)
{
	fprintf(stderr,"Syntax Error\n");
}

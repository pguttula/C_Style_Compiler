%{

#include <stdio.h>
#include<string.h>
#include "ast.h"

void yyerror(const char * s);
int yylex();
extern int yylineno;

//#define YYSTYPE struct ast_node*
struct ast_node* parser_result = NULL;

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
%token<id_val> TOKEN_ID
%token<str_val> TOKEN_STRING
%token TOKEN_ERROR
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_WHILE
%token TOKEN_LBRACS
%token TOKEN_RBRACS

/*%type <ast_node> program statments stat atom expression params
 param*/
%type <ast_node> program statments stat primexp expr exprmul
comp_stat expradd exprsub exprless exprcomp exprand expror params
param ifbody

%union {
		int	int_val;
    char *str_val;
    char *id_val;
    struct ast_node* ast_node;
	}

%locations

%%

program:
   statments {parser_result = $1;}
;

statments:
   stat {$$ = $1;}
   |
   stat statments {$$ =
   mk_seq_node(NODE_SEQ,$1,$2);
   }
;

stat:
   TOKEN_ID TOKEN_EQ expr TOKEN_SCOLON {$$ =
                                         mk_assign_node(NODE_ASSIGN,$1,$3);
   } 
   |
   TOKEN_PRINT TOKEN_LPAREN params TOKEN_RPAREN TOKEN_SCOLON {$$ =
                                         mk_print_node(NODE_PRINT,$3);
   }
   |
   TOKEN_PRINTLN TOKEN_LPAREN params TOKEN_RPAREN TOKEN_SCOLON {$$ = 
                                        mk_print_node(NODE_PRINTLN,$3);
   }
   |
  /* TOKEN_IF expr stat {$$ = mk_if_node(NODE_IF,$2,$3); }
   |*/
   TOKEN_IF expr ifbody {$$ = mk_if_node(NODE_IF,$2,$3); }
   |
   TOKEN_WHILE expr stat {$$ = 
                           mk_while_node(NODE_WHILE,$2,$3);
   }
   |
   comp_stat {$$ = $1; }
;

ifbody:
   stat {$$ = mk_ifbody_node(NODE_IFBODY,$1,NULL); } 
   |
   stat TOKEN_ELSE stat {$$ = mk_ifbody_node(NODE_IFBODY,$1,$3); }
;

comp_stat:
   TOKEN_LBRACS statments TOKEN_RBRACS {$$ = $2; }
;

primexp:
   TOKEN_STRING {$$ = mk_string_node($1); }
   |
   TOKEN_ID {$$ = mk_id_node($1); }
   |
   TOKEN_INT {$$ = mk_int_node($1); }
   |
   TOKEN_LPAREN expr TOKEN_RPAREN {$$ = $2; } 
 /*  |
    TOKEN_FOR TOKEN_ID TOKEN_IN TOKEN_LSQUARE TOKEN_INT TOKEN_COMMA TOKEN_INT TOKEN_RSQUARE TOKEN_SUM TOKEN_LPAREN expression TOKEN_RPAREN
;*/
;

exprmul:
    primexp {$$ = $1; }
    |
    primexp TOKEN_MUL exprmul {$$ =  mk_binop_node(NODE_MUL,$1,$3); }
;

expradd:
    exprmul {$$ = $1; }
    |
    exprmul TOKEN_ADD expradd {$$ = mk_binop_node(NODE_ADD,$1,$3); }
;

exprsub:
    expradd {$$ = $1; }
    |
    exprsub TOKEN_SUB expradd {$$ = mk_binop_node(NODE_SUB,$1,$3); }
;

exprless:
    exprsub {$$ = $1; }
    |
    exprsub TOKEN_LESS exprless {$$ = mk_binop_node(NODE_LESS,$1,$3); }
;

exprcomp:
    exprless {$$ = $1; }
    |
    exprless TOKEN_COMPARE exprcomp {$$ =
                                  mk_binop_node(NODE_COMPARE,$1,$3);}
;

exprand:
    exprcomp {$$ = $1; }
    |
    exprcomp TOKEN_AND exprand {$$ = mk_binop_node(NODE_AND,$1,$3); }
;

expror:
    exprand {$$ = $1; }
    |
    exprand TOKEN_OR expror {$$ = mk_binop_node(NODE_OR,$1,$3);}
;

expr:
    expror {$$ = $1; }
;

params:
    param {$$ = mk_param_node(NODE_PARAM,$1,NULL);}
    |
    param TOKEN_COMMA params {$$ = mk_param_node(NODE_PARAM,$1,$3); }
;

param:
    expr {$$ = $1; }
;

%%

void yyerror(const char * s)
{
	fprintf(stderr,"Syntax Error\n");
}

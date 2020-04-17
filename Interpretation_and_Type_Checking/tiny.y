%{

#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include "ast.h"

void yyerror(const char * s);
int yylex();
extern int yylineno;

//#define YYSTYPE struct ast_node*
struct ast_node* parser_result = NULL;

%}

%token<int_val> TOKEN_INT
/*%token<int_val> TOKEN_INTL
%token<int_val> TOKEN_INTU*/
%token<lineno> TOKEN_ADD
%token<lineno> TOKEN_SUB
%token<lineno> TOKEN_COMPARE
%token<lineno> TOKEN_LESS
%token<lineno> TOKEN_MUL
%token<lineno> TOKEN_AND
%token<lineno> TOKEN_OR
%token<lineno> TOKEN_PRINT
%token<lineno> TOKEN_PRINTLN
%token<lineno> TOKEN_FOR
%token<lineno> TOKEN_IN
%token<lineno> TOKEN_SCOLON
%token<lineno> TOKEN_COMMA
%token<lineno> TOKEN_LPAREN
%token<lineno> TOKEN_RPAREN
%token<lineno> TOKEN_LSQUARE
%token<lineno> TOKEN_RSQUARE
%token<lineno> TOKEN_SUM
%token<lineno> TOKEN_EQ
%token<id_val> TOKEN_ID
%token<str_val> TOKEN_STRING
%token<lineno> TOKEN_ERROR
%token<lineno> TOKEN_IF
%token<lineno> TOKEN_ELSE
%token<lineno> TOKEN_WHILE
%token<lineno> TOKEN_LBRACS
%token<lineno> TOKEN_RBRACS
%token<lineno> TOKEN_LESSEQUAL
%token<lineno> TOKEN_GREAT
%token<lineno> TOKEN_GREATEQUAL
%token<lineno> TOKEN_NOTEQUAL
%token<lineno> TRUE
%token<lineno> FALSE
%token<lineno> BOOL
%token<lineno> TOKEN_INTTYPE
%token<lineno> TOKEN_BOOLTYPE
%token<lineno> TOKEN_STRINGTYPE
%token<lineno> TOKEN_TRUE
%token<lineno> TOKEN_FALSE
%token<lineno> TOKEN_NULL
/*%type <ast_node> program statments stat atom expression params
 param*/
%type <ast_node> program statments stat primexp expr exprmul
comp_stat expradd exprsub exprless exprcomp exprand expror params
param ifbody exprlessequal exprgreat exprgreatequal exprnotequal
idlist typename id

%union {
    int lineno;
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
   mk_seq_node(NODE_SEQ,$1,$2); }
;

stat:
   typename idlist TOKEN_SCOLON {$$ = mk_decl_node(NODE_DECL,$1,$2,$3); }
   |
   id TOKEN_EQ expr TOKEN_SCOLON {$$ =
                                         mk_assign_node(NODE_ASSIGN,$1,$2,$3);
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
   TOKEN_IF expr ifbody {$$ = mk_if_node(NODE_IF,$1,$2,$3); }
   |
   TOKEN_WHILE expr stat {$$ = 
                           mk_while_node(NODE_WHILE,$2,$3,$1);
   }
   |
   comp_stat {$$ = mk_compound_stmt_node($1); }
;

typename:
   TOKEN_INTTYPE {$$ = mk_typenode(NODE_INTTYPE); }
   |
   TOKEN_BOOLTYPE {$$ = mk_typenode(NODE_BOOLTYPE); }
   |
   TOKEN_STRINGTYPE {$$ = mk_typenode(NODE_STRINGTYPE); }
;

id:
  TOKEN_ID {$$ = mk_id_node($1);}

idlist:
   id {$$ = $1;}
   |
   id TOKEN_COMMA idlist {$$ =  mk_seq_id(NODE_IDSEQ,$1,$3); }
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
   TOKEN_TRUE {$$ = mk_bool_node(true);}
   |
   TOKEN_FALSE {$$ = mk_bool_node(false);}
   |
   TOKEN_STRING {$$ = mk_string_node($1); }
   |
   TOKEN_NULL { $$ = mk_string_node(NULL); }
   |
   id {$$ = $1;}
   |
   TOKEN_INT {$$ = mk_int_node($1); }
   |
   TOKEN_LPAREN expr TOKEN_RPAREN {$$ = $2; } 
   |
    TOKEN_FOR id TOKEN_IN TOKEN_LSQUARE TOKEN_INT TOKEN_COMMA
    TOKEN_INT TOKEN_RSQUARE TOKEN_SUM TOKEN_LPAREN expr
    TOKEN_RPAREN {$$ = mk_for_node(NODE_FOR,$2,$5,$7,$11,$4,$9); }

;

exprmul:
    primexp {$$ = $1; }
    |
    exprmul TOKEN_MUL primexp {$$ =  mk_binop_node(NODE_MUL,$1,$2,$3); 
}
;

exprsub:
    exprmul {$$ = $1; }
    |
    exprsub TOKEN_SUB exprmul {$$ = mk_binop_node(NODE_SUB,$1,$2,$3); 
}
;

expradd:
    exprsub {$$ = $1; }
    |
    expradd TOKEN_ADD exprsub {$$ = mk_binop_node(NODE_ADD,$1,$2,$3);
}
;

exprless:
    expradd {$$ = $1; }  
    |
    exprless TOKEN_LESS expradd {$$ = mk_binop_node(NODE_LESS,$1,$2,$3); 
}
;

exprlessequal:
    exprless {$$ = $1; }
    |
    exprlessequal TOKEN_LESSEQUAL exprless {$$ = mk_binop_node(NODE_LESSEQUAL,$1,$2,$3); 
}
;

exprgreat:
    exprlessequal {$$ = $1; }
    |
    exprgreat TOKEN_GREAT exprlessequal {$$ = mk_binop_node(NODE_GREAT,$1,$2,$3);
}
;

exprgreatequal:
    exprgreat {$$ = $1; }
    |
    exprgreatequal TOKEN_GREATEQUAL exprgreat {$$ = mk_binop_node(NODE_GREATEQUAL,$1,$2,$3);
}
;

exprcomp:
    exprgreatequal {$$ = $1; }
    |
    exprcomp TOKEN_COMPARE exprgreatequal {$$ = mk_binop_node(NODE_COMPARE,$1,$2,$3);
}
;

exprnotequal:
    exprcomp {$$ = $1; }
    |
    exprnotequal TOKEN_NOTEQUAL exprcomp {$$ = mk_binop_node(NODE_NOTEQUAL,$1,$2,$3);
}
;
exprand:
    exprnotequal {$$ = $1; }
    |
    exprand TOKEN_AND exprnotequal { $$ = mk_binop_node(NODE_AND,$1,$2,$3); 
}
;

expror:
    exprand {$$ = $1; }
    |
    expror TOKEN_OR exprand {$$ = mk_binop_node(NODE_OR,$1,$2,$3);
}
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

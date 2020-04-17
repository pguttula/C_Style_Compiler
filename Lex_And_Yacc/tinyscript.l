%{

#include "y.tab.h"
#include <stdio.h>

extern YYSTYPE yylval;

void yyerror(const char* s);


%}
%x BLOCK_COMMENT

%%

"/*" { BEGIN(BLOCK_COMMENT); }
<BLOCK_COMMENT>"*/" { BEGIN(INITIAL); }
<BLOCK_COMMENT>[^*]
<BLOCK_COMMENT>"*"
<BLOCK_COMMENT><<EOF>> {
      return TOKEN_ERROR;
 }
"//".* { /*LINE_COMMENT*/ }


[ \t\n]+ 	{	}


"+"		{ return TOKEN_ADD; }
"-"		{ return TOKEN_SUB; }
"=="  { return TOKEN_COMPARE; }
"<"   { return TOKEN_LESS; }
"*"   { return TOKEN_MUL; }
"&&"  { return TOKEN_AND; }
"||"  { return TOKEN_OR; }
"Print" { return TOKEN_PRINT; }
"Println" { return TOKEN_PRINTLN; }
"for" { return TOKEN_FOR; }
"in"  { return TOKEN_IN; }
";"   { return TOKEN_SCOLON; }
","   { return TOKEN_COMMA; }
"("   { return TOKEN_LPAREN; }
")"   { return TOKEN_RPAREN; }
"["   { return TOKEN_LSQUARE; }
"]"   { return TOKEN_RSQUARE; }
"SUM" { return TOKEN_SUM; }
"="   { return TOKEN_EQ; }

[0-9][0-9]*	{
		int i = 0;
		yylval.int_val = atoi(yytext);
		return TOKEN_INT;
}

[a-zA-Z][_a-zA-Z0-9]* {
    
    yylval.id_val = strdup(yytext);
    return TOKEN_ID;
}


\"[^"]*\" {
    yylval.str = strdup(yytext);
    return TOKEN_STRING;
}

.    { 
   /* yyerror(yytext);*/
        return TOKEN_ERROR;
    }



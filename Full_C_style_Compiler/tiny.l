%{

#include "y.tab.h"
#include <stdio.h>

extern YYSTYPE yylval;

void yyerror(const char* s);


%}

%option yylineno
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


"+"		{ yylval.lineno=yylineno;
        return TOKEN_ADD; }
"-"		{ yylval.lineno=yylineno;
        return TOKEN_SUB; }
"=="  { yylval.lineno=yylineno;
        return TOKEN_COMPARE; }
"!="  { yylval.lineno=yylineno;
        return TOKEN_NOTEQUAL; }
"<="  { yylval.lineno=yylineno;
        return TOKEN_LESSEQUAL; }
">"   { yylval.lineno=yylineno;
        return TOKEN_GREAT; }
">="  { yylval.lineno=yylineno;
        return TOKEN_GREATEQUAL; }
"<"   { yylval.lineno=yylineno;
        return TOKEN_LESS; }
"*"   { yylval.lineno=yylineno;
        return TOKEN_MUL; }
"&&"  { yylval.lineno=yylineno;
        return TOKEN_AND; }
"||"  { yylval.lineno=yylineno;
        return TOKEN_OR; }
"Print" { yylval.lineno=yylineno;
        return TOKEN_PRINT; }
"Println" { yylval.lineno=yylineno;
        return TOKEN_PRINTLN; }
"Input" { yylval.lineno=yylineno;
        return TOKEN_INPUT; }
"for" { yylval.lineno=yylineno;
        return TOKEN_FOR; }
"in"  { yylval.lineno=yylineno;
        return TOKEN_IN; }
";"   { yylval.lineno=yylineno;
        return TOKEN_SCOLON; }
","   { yylval.lineno=yylineno;
        return TOKEN_COMMA; }
"("   { yylval.lineno=yylineno;
        return TOKEN_LPAREN; }
")"   { yylval.lineno=yylineno;
        return TOKEN_RPAREN; }
"["   { yylval.lineno=yylineno;
        return TOKEN_LSQUARE; }
"]"   { yylval.lineno=yylineno;
        return TOKEN_RSQUARE; }
"SUM" { yylval.lineno=yylineno;
        return TOKEN_SUM; }
"="   { yylval.lineno=yylineno; 
        return TOKEN_EQ; }
"if"  { yylval.lineno=yylineno;
        return TOKEN_IF; }
"else"  {return TOKEN_ELSE; }
"while" { yylval.lineno=yylineno;
        return TOKEN_WHILE; }
"{"   { yylval.lineno=yylineno;
        return TOKEN_LBRACS; }
"}"   { yylval.lineno=yylineno;
        return TOKEN_RBRACS; }
"int" { yylval.lineno=yylineno;
        return TOKEN_INTTYPE; }
"bool" { yylval.lineno=yylineno;
        return TOKEN_BOOLTYPE; }
"string" { yylval.lineno=yylineno;
        return TOKEN_STRINGTYPE; }
"true" { yylval.lineno=yylineno;
        return TOKEN_TRUE; }
"false" { yylval.lineno=yylineno;
        return TOKEN_FALSE; }
"null"  {yylval.lineno=yylineno;
        return TOKEN_NULL; }
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
    int i;
    int len = strlen(yytext);
    char *str = strdup(yytext);
    for(i=1; i<len-1; i++) {
      str[i-1]=str[i];
    }
    str[i-1]='\0';
    yylval.str_val = str;
    return TOKEN_STRING;
}

.    { 
   /* yyerror(yytext);*/
        return TOKEN_ERROR;
    }



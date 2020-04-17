/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_INT = 258,
    TOKEN_ADD = 259,
    TOKEN_SUB = 260,
    TOKEN_COMPARE = 261,
    TOKEN_LESS = 262,
    TOKEN_MUL = 263,
    TOKEN_AND = 264,
    TOKEN_OR = 265,
    TOKEN_PRINT = 266,
    TOKEN_PRINTLN = 267,
    TOKEN_INPUT = 268,
    TOKEN_FOR = 269,
    TOKEN_IN = 270,
    TOKEN_SCOLON = 271,
    TOKEN_COMMA = 272,
    TOKEN_LPAREN = 273,
    TOKEN_RPAREN = 274,
    TOKEN_LSQUARE = 275,
    TOKEN_RSQUARE = 276,
    TOKEN_SUM = 277,
    TOKEN_EQ = 278,
    TOKEN_ID = 279,
    TOKEN_STRING = 280,
    TOKEN_ERROR = 281,
    TOKEN_IF = 282,
    TOKEN_ELSE = 283,
    TOKEN_WHILE = 284,
    TOKEN_LBRACS = 285,
    TOKEN_RBRACS = 286,
    TOKEN_LESSEQUAL = 287,
    TOKEN_GREAT = 288,
    TOKEN_GREATEQUAL = 289,
    TOKEN_NOTEQUAL = 290,
    TRUE = 291,
    FALSE = 292,
    BOOL = 293,
    TOKEN_INTTYPE = 294,
    TOKEN_BOOLTYPE = 295,
    TOKEN_STRINGTYPE = 296,
    TOKEN_TRUE = 297,
    TOKEN_FALSE = 298,
    TOKEN_NULL = 299
  };
#endif
/* Tokens.  */
#define TOKEN_INT 258
#define TOKEN_ADD 259
#define TOKEN_SUB 260
#define TOKEN_COMPARE 261
#define TOKEN_LESS 262
#define TOKEN_MUL 263
#define TOKEN_AND 264
#define TOKEN_OR 265
#define TOKEN_PRINT 266
#define TOKEN_PRINTLN 267
#define TOKEN_INPUT 268
#define TOKEN_FOR 269
#define TOKEN_IN 270
#define TOKEN_SCOLON 271
#define TOKEN_COMMA 272
#define TOKEN_LPAREN 273
#define TOKEN_RPAREN 274
#define TOKEN_LSQUARE 275
#define TOKEN_RSQUARE 276
#define TOKEN_SUM 277
#define TOKEN_EQ 278
#define TOKEN_ID 279
#define TOKEN_STRING 280
#define TOKEN_ERROR 281
#define TOKEN_IF 282
#define TOKEN_ELSE 283
#define TOKEN_WHILE 284
#define TOKEN_LBRACS 285
#define TOKEN_RBRACS 286
#define TOKEN_LESSEQUAL 287
#define TOKEN_GREAT 288
#define TOKEN_GREATEQUAL 289
#define TOKEN_NOTEQUAL 290
#define TRUE 291
#define FALSE 292
#define BOOL 293
#define TOKEN_INTTYPE 294
#define TOKEN_BOOLTYPE 295
#define TOKEN_STRINGTYPE 296
#define TOKEN_TRUE 297
#define TOKEN_FALSE 298
#define TOKEN_NULL 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 68 "tiny.y" /* yacc.c:1909  */

    int lineno;
		int	int_val;
    char *str_val;
    char *id_val;
    struct ast_node* ast_node;
	

#line 151 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

#include <stdio.h>
#include "interp.h"
#include "typecheck.h"

int yyparse();
extern struct ast_node* parser_result;

int main()
{
  if(yyparse()==1) {
   // printf("Parse Error!\n");
    return 0;
  }
  //traverse_and_print_ast(parser_result);
  type_check(parser_result);
  int result = interpret(parser_result);
  //printf("%d\n",result);
  return 0;
};

#include <stdio.h>
#include "interp.h"
#include "typecheck.h"
#include "dataflow.h"

int yyparse();
extern struct ast_node* parser_result;
extern int err_count;

int main()
{
  if(yyparse()==1) {
   // printf("Parse Error!\n");
    return 0;
  }
  //traverse_and_print_ast(parser_result);
  type_check(parser_result);

  data_flow_stmt(parser_result);
  //if(err_count <= 0) {
   // int result = interpret(parser_result);
  //}
  //printf("%d\n",result);
  return 0;
};

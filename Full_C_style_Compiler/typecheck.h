#include "ast.h"


typedef enum{
  TYPE_INT,
  TYPE_BOOL,
  TYPE_STRING
} type_t;

extern type_t type_check(struct ast_node*);

typedef enum {
  NODE_ID,
  NODE_INT,
  NODE_STRING,
  NODE_SEQ,
  NODE_ASSIGN,
  NODE_PRINT,
  NODE_PRINTLN,
  NODE_PARAM,
  NODE_IF,
  NODE_IFBODY,
  NODE_WHILE,
  NODE_ADD,
  NODE_SUB,
  NODE_LESS,
  NODE_COMPARE,
  NODE_AND,
  NODE_OR,
  NODE_MUL
} node_t;

struct ast_node {
  node_t kind;
  int int_val;
  char *str_val;
  char *id_val;
  struct ast_node* left;
  struct ast_node* right;
  int lineno;
};

extern struct ast_node* mk_id_node(char*);
extern struct ast_node* mk_int_node(int);
extern struct ast_node* mk_string_node(char*);
extern struct ast_node* mk_binop_node(node_t binop, 
    struct ast_node*, struct ast_node*);
extern void traverse_and_print_ast(struct ast_node*);
extern struct ast_node* mk_assign_node(node_t assign,
    char*, struct ast_node*);
extern struct ast_node* mk_seq_node(node_t seq,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_if_node(node_t ifloop,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_while_node(node_t whilel,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_print_node(node_t printloop,
    struct ast_node*);
extern struct ast_node* mk_param_node(node_t params,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_ifbody_node(node_t ifbody,
    struct ast_node*, struct ast_node*);

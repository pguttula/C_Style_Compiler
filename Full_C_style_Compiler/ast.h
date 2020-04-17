typedef enum {
  NODE_ID,
  NODE_INT,
  NODE_BOOL,
  NODE_STRING,
  NODE_SEQ,
  NODE_ASSIGN,
  NODE_PRINT,
  NODE_PRINTLN,
  NODE_PARAM,
  NODE_PARAMLN,
  NODE_INPUT,
  NODE_FOR,
  NODE_IF,
  NODE_IFBODY,
  NODE_WHILE,
  NODE_ADD,
  NODE_SUB,
  NODE_LESS,
  NODE_COMPARE,
  NODE_AND,
  NODE_OR,
  NODE_MUL,
  NODE_LESSEQUAL,
  NODE_GREAT,
  NODE_GREATEQUAL,
  NODE_NOTEQUAL,
  NODE_DECL,
  NODE_IDSEQ,
  NODE_INTTYPE,
  NODE_BOOLTYPE,
  NODE_STRINGTYPE,
  NODE_COMPOUND_STMT
} node_t;

struct ast_node {
  node_t kind;
  int forlbound;
  int forubound;
  int int_val;
  bool bool_val;
  char *str_val;
  char *id_val;
  struct ast_node* left;
  struct ast_node* right;
  int lineno;
  int slineno;
};

extern struct ast_node* mk_id_node(char*);
extern struct ast_node* mk_int_node(int);
extern struct ast_node* mk_string_node(char*);
extern struct ast_node* mk_binop_node(node_t binop, 
    struct ast_node*, int, struct ast_node*);
extern void traverse_and_print_ast(struct ast_node*);
extern struct ast_node* mk_assign_node(node_t assign,
    struct ast_node*, int,struct ast_node*);
extern struct ast_node* mk_seq_node(node_t seq,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_if_node(node_t ifloop,int,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_for_node(node_t forloop,
    struct ast_node*, int ,int ,struct ast_node*,int,int);
extern struct ast_node* mk_while_node(node_t whilel,
    struct ast_node*, struct ast_node*,int);
extern struct ast_node* mk_print_node(node_t printloop,
    struct ast_node*);
extern struct ast_node* mk_input_node(node_t input,
    struct ast_node*);
extern struct ast_node* mk_param_node(node_t params,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_ifbody_node(node_t ifbody,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_decl_node(node_t decl,
    struct ast_node*, struct ast_node*,int);
extern struct ast_node* mk_seq_id(node_t idseq,
    struct ast_node*, struct ast_node*);
extern struct ast_node* mk_typenode(node_t type);
extern struct ast_node* mk_bool_node(bool);
extern struct ast_node* mk_compound_stmt_node(struct ast_node*);

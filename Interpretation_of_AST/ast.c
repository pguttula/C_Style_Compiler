#include<stdlib.h>
#include<stdio.h>
#include "ast.h"

extern int yylineno;

struct ast_node* mk_node() {
  struct ast_node* new_node = (struct ast_node*)malloc(sizeof(struct ast_node));
  new_node->lineno = yylineno;
  return new_node;
}

struct ast_node* mk_id_node(char* id_val) {
  struct ast_node* v = mk_node();
  v->kind = NODE_ID;
  v->id_val = id_val;
  return v;
}

struct ast_node* mk_int_node(int int_val) {
  struct ast_node* v = mk_node();
  v->kind = NODE_INT;
  v->int_val = int_val;
  return v;
}

struct ast_node* mk_string_node(char* str_val) {
  struct ast_node* v=mk_node();
  v->kind = NODE_STRING;
  v->str_val = str_val;
  return v;
}

struct ast_node* mk_binop_node(node_t binop, struct ast_node* arg1,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = binop;
  v->left = arg1;
  v->right = arg2;
  return v;
}

struct ast_node* mk_seq_node(node_t seq, struct ast_node* arg1,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = seq;
  v->left = arg1;
  v->right = arg2;
  return v;
}

struct ast_node* mk_assign_node(node_t assign, char* id_val,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = assign;
  v->id_val = id_val;
  v->right = arg2;
  return v;
}

struct ast_node* mk_if_node(node_t ifloop, struct ast_node* arg1,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = ifloop;
  v->left = arg1;
  v->right = arg2;
  return v;
}

struct ast_node* mk_ifbody_node(node_t ifbody, struct ast_node* arg1,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = ifbody;
  v->left = arg1;
  v->right = arg2;
  return v;
}
struct ast_node* mk_while_node(node_t whilel, struct ast_node* arg1,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = whilel;
  v->left = arg1;
  v->right = arg2;
  return v;
}

struct ast_node* mk_param_node(node_t params, struct ast_node* arg1,
    struct ast_node* arg2) {
  struct ast_node* v = mk_node();
  v->kind = params;
  v->left = arg1;
  v->right = arg2;
  return v;
}

struct ast_node* mk_print_node(node_t printloop, struct ast_node* arg1){
  struct ast_node* v = mk_node();
  v->kind = printloop;
  v->left = arg1;
  v->right = NULL;
  return v;
}

void traverse_and_print_ast(struct ast_node* root) {
  if (root == NULL)  {
    return;
  }
  switch(root->kind) {
      case NODE_ID:
      printf("ID(%s)\n",root->id_val);
      break;
    case NODE_INT:
      printf("INT(%d)\n",root->int_val);
      break;
    case NODE_STRING:
      printf("STRING(%s)\n", root->str_val);
      break;
    case NODE_SEQ:
      printf("SEQ\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_PARAM:
      printf("PARAM\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_PRINT:
      printf("PRINT\n");
      traverse_and_print_ast(root->left);
      break;
    case NODE_PRINTLN:
      printf("PRINTLN\n");
      traverse_and_print_ast(root->left);
      break;
    case NODE_ASSIGN:
     /* printf("ASSIGNMENT\n");*/
      printf("ID(%s)\n",root->id_val);
      traverse_and_print_ast(root->right);
      break;
    case NODE_IF:
      printf("IF\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_IFBODY:
      printf("IFBODY\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_WHILE:
      printf("WHILE\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_ADD:
      printf("ADD\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_SUB:
      printf("SUB\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_LESS:
      printf("LESS\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_COMPARE:
      printf("COMPARE\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_AND:
      printf("AND\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_OR:
      printf("OR\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    case NODE_MUL:
      printf("MUL\n");
      traverse_and_print_ast(root->left);
      traverse_and_print_ast(root->right);
      break;
    default:
      printf("Parse Error\n");
      break;
  }
  return;
}

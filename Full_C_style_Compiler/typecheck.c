#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "ast.h"

typedef enum{
  TYPE_INT,
  TYPE_BOOL,
  TYPE_STRING,
  TYPE_ERROR
} type_t;

struct tt_node{
  char* id;
  type_t typ;
};

#define TYPE_TAB_SIZE 1000

struct tt_node* type_tab[TYPE_TAB_SIZE];
static short count = 0;

static int tt_stack[100];
static int tt_stack_top=-1;

/* TYPE TABLE MANAGEMENT */

static int lookup_typ(char* id) {
  //printf("Looking up %s.. ",id);
  for(int i=count-1; i>=0; i--) {
    struct tt_node* node = type_tab[i];
    if(strcmp(id,node->id) == 0) {
      //printf("found\n");
      return i;
    }
  }
  //printf("not found\n");
  return -1;
}

static int lookup_typ_in_cur_scope(char* id) {
  //For the current scope, type table beginning index 
  // is tt_stack[tt_stack_top].
  //printf("%d",count);
  //
  int first = tt_stack[tt_stack_top];
  //printf("%d",first);
  for(int i=count-1; i>=first; i--) {
    struct tt_node* node = type_tab[i];
    if(strcmp(id,node->id) == 0) {
      return i;
    }
  }
  return -1;
}

static void add_id_typ(char* id, type_t typ) {
  // the index at which you insert is `count`
  int new_index = count;
  // We assume that count never exeeds TYPE_TAB_SIZE. 
  // TODO: get rid of this assumption.
  count++;
  // create a new node to store key and value
  struct tt_node* new_node = (struct tt_node*)malloc(sizeof(struct tt_node));
  new_node->id= id;
  new_node->typ = typ;
  // now, insert the node into the symbol table.
  type_tab[new_index] = new_node;
  return;
  }

void print_tt() {
  int i;
  printf("----- Type Table ----\n");
  for(i=0;i<count;i++) {
    printf("%s -> %d\n",type_tab[i]->id, type_tab[i]->typ);
  }
  printf("----- Type Table ----\n");
}

/* TYPE TABLE STACK MANAGEMENT */

void push_tt() {
  tt_stack_top++;
  //push current size of type table on stack.
  tt_stack[tt_stack_top] = count;
  return;
}

void pop_tt() {
  if (tt_stack_top < 0) {
    printf("pop_tt unexpectedly called!\n");
    exit(0);
  }
  //restore the previous size of the type table
  count = tt_stack[tt_stack_top];
  tt_stack_top--;
  return;
}

type_t type_of_decl_node(struct ast_node* node) {
  switch(node->kind) {
    case NODE_INTTYPE:
      return TYPE_INT;
    break;

    case NODE_BOOLTYPE:
      return TYPE_BOOL;
    break;

    case NODE_STRINGTYPE:
      return TYPE_STRING;
    break;

    default:
      printf("Unexpected LHS in a declaration!");
      exit(0);
    break;
  }
}

/* TYPE ERROR HANDLING */

int errors[1000];
int err_count = 0;

void error(int lineno) {
  int i;
  for(i=0;i<=err_count;i++) {
    if (errors[i] == lineno)
      return;
  }
  errors[err_count]=lineno;
  err_count++;
  //printf("Type violation in %d\n",lineno);
  return;
}
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void bubble_sort(int arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)      
 
       // Last i elements are already in place   
       for (j = 0; j < n-i-1; j++) 
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

void print_all_errors() {
  int i;
  bubble_sort(errors,err_count);	
  for(i=0;i<err_count;i++) {
    printf("Type violation in %d\n",errors[i]);
  }
}

static int lookup_input(char* id){
  int first = tt_stack[tt_stack_top];
  for(int i=count-1;i>=0; i--) {
    struct tt_node* node = type_tab[i];
    if(strcmp(id,node->id) == 0) {
      return i;
    }
  }
  return -1;
}
static void check_decl_input(struct ast_node* node,int lineno){
  switch(node->kind) {
  case NODE_ID:{
    char* id = node->id_val;
    //printf("%s, %d \n",id, lineno);
  //  printf("%s,%d,%d\n",id,count,tt_stack[tt_stack_top]);
    if(lookup_input(id) < 0){
      error(lineno);
    }
  }
    return;
  break;
  case NODE_IDSEQ:{
    check_decl_input(node->left,lineno);
    check_decl_input(node->right,lineno);
  break;
  }
  }
}
void declare_ids_of_typ(type_t typ,int lineno, struct ast_node* node) {

  switch(node->kind) {

    case NODE_ID: {
      char* id = node->id_val;
      if(lookup_typ_in_cur_scope(id) < 0){
        add_id_typ(id,typ);
      }
      else{
        error(lineno);
      }
      return;
    }
    break;
    case NODE_IDSEQ:
      //add_id_typ(id,typ);
      declare_ids_of_typ(typ,lineno,node->left);	
      declare_ids_of_typ(typ,lineno,node->right);
      return;
    break;
    default:
      printf("Unexpected RHS in the declaration!\n");
      exit;
    break;
  }
}
type_t type_check_expr(struct ast_node* root) {
  int i,j;
  type_t typ, typ1, typ2;

  if (root == NULL)  {
    printf("type_check_expr called on NULL!\n");
    exit(0);
  }

  switch(root->kind) {

    case NODE_INT:
      return TYPE_INT;
    break;

    case NODE_BOOL:
      return TYPE_BOOL;
    break;
    
    case NODE_STRING:
      return TYPE_STRING;
    break;

    case NODE_ID:
      i = lookup_typ(root->id_val);
      //printf("%d %s",i,root->id_val);
      if (i<0) {
        error(root->lineno);
        return TYPE_ERROR;
      }
      typ = type_tab[i]->typ;
      j = lookup_typ_in_cur_scope(root->id_val);
      if(j<0) {
        //id is being used in the current scope with the same type 
        // as previous scope. So, lets record that fact.
        add_id_typ(root->id_val,typ);
      }
      if (i>=0 && j>=0 && i!=j) {
        printf("Warning: Something is wrong with type lookup." 
            "lookup_typ should've found the typ in the current "
            "scope, but it found something else!\n");
      }
      return typ;
    break;

    case NODE_PARAM:
    {
      typ1 = type_check_expr(root->left);
      if(root->right != NULL) {
        type_check_expr(root->right);
        
      }else{

      return typ1;
      }
    }
    break;
    
    case NODE_PARAMLN:
    {
      typ1 = type_check_expr(root->left);
      if(root->right != NULL) {
        type_check_expr(root->right);
      }else{
        return typ1;
      }
    }
    break;
    case NODE_ADD:
    case NODE_SUB:
    case NODE_MUL:
      typ1 = type_check_expr(root->left);
      typ2 = type_check_expr(root->right);
      if(typ1 == TYPE_INT && typ2 == TYPE_INT) {
        return TYPE_INT;
      } 
      else if (typ1 == TYPE_ERROR || typ2 == TYPE_ERROR) {
        return TYPE_INT;
      }
      else{
        error(root->lineno);
        return TYPE_ERROR;
      }
    break;

    case NODE_LESS:
    case NODE_LESSEQUAL:
    case NODE_GREAT:
   case NODE_GREATEQUAL:
    case NODE_COMPARE:
    case NODE_NOTEQUAL:
      typ1 = type_check_expr(root->left);
      typ2 = type_check_expr(root->right);
      if(typ1  == TYPE_INT && typ2 == TYPE_INT){
        return TYPE_BOOL;
      }
      else if (typ1 == TYPE_ERROR || typ2 == TYPE_ERROR) {
        return TYPE_BOOL;
      }
      else {
        error(root->lineno);
        return TYPE_ERROR;
      } 
    break;

    case NODE_AND:
      typ1 = type_check_expr(root->left);
      typ2 = type_check_expr(root->right);
      if((typ1 == TYPE_BOOL)
          && (typ2 == TYPE_BOOL)){
          return TYPE_BOOL;
      } 
      else if (typ1 == TYPE_ERROR || typ2 == TYPE_ERROR) {
        return TYPE_BOOL;
      }
      else {
        error(root->lineno);
        return TYPE_ERROR;
      } 
    break;
    
    case NODE_OR:{
      typ1 = type_check_expr(root->left);
      typ2 = type_check_expr(root->right);
      if((typ1 == TYPE_BOOL)
          && (typ2 == TYPE_BOOL)){
          return TYPE_BOOL;
      }
      else if((typ1 == TYPE_STRING)
          && (typ2 == TYPE_STRING)){
        return TYPE_STRING;
      } 
      else if ((typ1 == TYPE_ERROR || typ2 == TYPE_ERROR)) {
        return TYPE_STRING;;
      }
      else {
        error(root->lineno);
        return TYPE_ERROR;
      }
                 }
    break;

    case NODE_FOR: {
      bool is_err = false;
      char *id = root->left->id_val;
      struct ast_node* expr = root->right;
      push_tt();
      add_id_typ(id,TYPE_INT);
      typ = type_check_expr(expr);
      pop_tt();
      int lb = root->forlbound;
      int ub = root->forubound;
    
      if(lb > ub) {
        error(root->lineno);
        is_err = true;
      }

      if(typ == TYPE_ERROR){
      //  printf("hi there");
        return TYPE_ERROR;
      }
      else if(typ != TYPE_INT) {
        //printf("hi there");
        error(root->slineno);
       // printf("%d", root->slineno);
        is_err = true;
        return TYPE_ERROR;
      }
       else if(is_err) {
        return TYPE_ERROR;
      } else {
        return TYPE_INT;
      }
    }
    break;

    default:
      printf("Unexpected expression encountered!\n");
      exit(0);
  }
}

void type_check_stmt(struct ast_node* root) {
  int i,j;
  type_t typ, i_typ, grd_typ;

  if (root == NULL)  {
    return;
  }
  
  switch (root->kind) {
    case NODE_SEQ:
       type_check_stmt(root->left);
       type_check_stmt(root->right);
    break;
    
    case NODE_DECL:
      typ = type_of_decl_node(root->left);
      declare_ids_of_typ(typ,root->lineno, root->right);
     
      //print_tt();
    break;
    
    case NODE_PRINT:
      type_check_expr((root->left));
    break;

    case NODE_PRINTLN:
      type_check_expr((root->left));

    break;

    case NODE_ASSIGN:{
      type_t lhs_typ = type_check_expr(root->left);
      type_t rhs_typ = type_check_expr(root->right);
      
      if(lhs_typ == TYPE_ERROR || rhs_typ == TYPE_ERROR) {
        return;
      } 
      if(lhs_typ != rhs_typ){
         error(root->lineno);
      } 
    }
    break;

    case NODE_IF:
     grd_typ = type_check_expr(root->left);
      
     if(grd_typ == TYPE_ERROR){
       return;
     }
      
     else if( grd_typ != TYPE_BOOL) {
       error(root->lineno);
        //printf("%d", root->lineno);
     }
     type_check_stmt(root->right);
               
    break;
    
    case NODE_WHILE:
      grd_typ = type_check_expr(root->left);
      if(grd_typ != TYPE_BOOL) {
        error(root->lineno);
        //printf("%d",root->lineno);
      }
      type_check_stmt(root->right);
    break;

    case NODE_IFBODY:
      type_check_stmt(root->left);
      if(root->right != NULL) {
        type_check_stmt(root->right);
      }
    break;

    case NODE_COMPOUND_STMT:
      if(root->right!=NULL || root->left == NULL) {
        printf("NODE_COMPOUND_STMT defn changed\n");
        exit(0);
      }
      push_tt();
      type_check_stmt(root->left);
      pop_tt();
    break;
    
    case NODE_INPUT:
    push_tt();
    check_decl_input(root->left,root->lineno);
   // printf("here");
    pop_tt();
    break;

    default:
      printf("Unexpected node encountered\n");
      exit(0);
    }
}

void type_check(struct ast_node* prog) {
  type_check_stmt(prog);
  print_all_errors();
  
}

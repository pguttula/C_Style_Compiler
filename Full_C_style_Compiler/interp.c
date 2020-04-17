#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "typecheck.h"

struct sym_node {
  char* key;
  type_t typ;
  int val;
  char* str_val;
  bool bool_val;
  
};

#define SYM_TAB_SIZE 1000

struct sym_node* sym_tab[SYM_TAB_SIZE];

static short count = 0;

static int lookup(char* key) {
  for(int i=count-1; i>0; i--) {
    // get the node at sym_tab[i]
    struct sym_node* node = sym_tab[i];
    if(strcmp(key,node->key) == 0) {
      return i;
    }
  }
  return -1;
}

static int val_stack[100];
static int val_stack_top=-1;

void push_val() {
  val_stack_top++;
  val_stack[val_stack_top] = count;
  return;
}
void pop_val() {
if(val_stack_top < 0) {
  printf("pop_val unexpectedly called!\n");
  exit(0);
  }
  count = val_stack[val_stack_top];
  val_stack_top--;
  return;
}

type_t type_of_decl(struct ast_node* node) {
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
static void add_decl_key(char* key, type_t typ){
  int new_index = count;
  count++;
  struct sym_node* new_node = (struct sym_node*)malloc(sizeof(struct sym_node));
  new_node->key = key;
  new_node->typ = typ;
  new_node->val = 123456789;
  //new_node->str_val = "pav";
  new_node->bool_val = 123456789;
  sym_tab[new_index] = new_node;
}

void declare_id(type_t typ,struct ast_node* node){
  switch(node->kind) {
    case NODE_ID:{
    char* id = node->id_val;
    add_decl_key(id,typ);
    return;
                 }
    break;
    
    case NODE_IDSEQ:
    declare_id(typ,node->left);
    declare_id(typ,node->right);
    return;
    break;

    default:
    printf("Unexpected RHS in the declaration!\n");
    exit;
    break;
  }
}

static void add_key_value(char* key, int val) {
  int index = lookup(key);
  if (index >= 0) {
    // its a valid index
    struct sym_node* node = sym_tab[index];
    node->typ = TYPE_INT;
    node->val = val;
    node->str_val = NULL;
    return;
  } else {
    // the index at which you insert is `count`
    int new_index = count;
    // We assume that count never exeeds SYM_TAB_SIZE. 
    // TODO: get rid of this assumption.
    count++;
    // create a new node to store key and value
    struct sym_node* new_node = (struct sym_node*)malloc(sizeof(struct sym_node));
    new_node->key = key;
    new_node->typ = TYPE_INT;
    new_node->val = val;
    new_node->str_val = NULL;
    // now, insert the node into the symbol table.
    sym_tab[new_index] = new_node;
  }
  return;
}

void add_key_value_str(char* key, char* str_val) {
  int index = lookup(key);
  if (index >= 0) {
    // its a valid index
    struct sym_node* node = sym_tab[index];
    node->typ = TYPE_STRING;
    node->str_val = str_val;
    return;
  } else {
    // the index at which you insert is `count`
    int new_index = count;
    // We assume that count never exeeds SYM_TAB_SIZE. 
    // TODO: get rid of this assumption.
    count++;
    // create a new node to store key and value
    struct sym_node* new_node = (struct sym_node*)malloc(sizeof(struct sym_node));
    new_node->key = key;
    new_node->typ = TYPE_STRING;
    new_node->str_val = str_val;
    // now, insert the node into the symbol table.
    sym_tab[new_index] = new_node;
  }
  return;
}

void add_key_value_bool(char* key, bool bool_val) {
  int index = lookup(key);
  
  if (index >= 0) {
    // its a valid index
    struct sym_node* node = sym_tab[index];
    node->typ = TYPE_BOOL;
    node->bool_val = bool_val;
    return;
  } else {
    // the index at which you insert is `count`
    int new_index = count;
    // We assume that count never exeeds SYM_TAB_SIZE. 
    // TODO: get rid of this assumption.
    count++;
    // create a new node to store key and value
    struct sym_node* new_node = (struct sym_node*)malloc(sizeof(struct sym_node));
    new_node->key = key;
    new_node->typ = TYPE_BOOL;
    new_node->bool_val = bool_val;
    // now, insert the node into the symbol table.
    sym_tab[new_index] = new_node;
  }
  return;
}



bool is_ret_str; //set to true if the return value is a string.
bool is_ret_bool; //set to true if the return value is a boolean
bool is_ret_int; //set to true if the return value is a int
bool iret_bool; //boolean return value.
int iret_int; // the integer return value.
char* iret_str; // the string return value.
const char* print_sep = " "; // seperator between params while printing. Space is the seperator for Print. Newline ("\n") for Println.
bool is_elsestat; //sets the else flag to TRUE if there is any else statement

void return_bool(bool b,int lineno) {
  is_ret_str = false;
  is_ret_int = false;
  is_ret_bool = true;
  iret_bool = b;
}

void return_int(int x,int lineno) {
  is_ret_str = false;
  is_ret_int = true;
  is_ret_bool = false;
  if(x != 123456789){
  iret_int = x;
  }

}

void return_string(char* s) {
  is_ret_str = true;
  is_ret_int = false;
  is_ret_bool = false;
  iret_str = s;
}



int interpret(struct ast_node* root){
  //TODO: impement
 if (root == NULL)  {
    return 0;
 }

 int i,j,sum;
 type_t typ;
  switch (root->kind) {
    case NODE_SEQ:
      interpret(root->left);
      // Whatever return vals set during interpretation of root->right
      // remain valid for root because we the result of a sequence of
      // commands is the result of the last command.
      return interpret(root->right);
    break;

    case NODE_INT:
      // return value must be int
      //printf("%d",root->lineno);
      return_int(root->int_val,root->lineno);
      return root->int_val;
    break;

    case NODE_BOOL:
      return_bool(root->bool_val,root->lineno);
      return root->bool_val;
    break;

    case NODE_STRING:
      //return value must be a string
      return_string(root->str_val);
      return -1;
    break;

    case NODE_ID:
      i=lookup(root->id_val);
      if(i > -1){
        struct sym_node* node =sym_tab[i];
        if(node->typ == TYPE_INT) {
          // The value of an id is an integer
          //printf("here %d\n",node->val);
          if(node->val != 123456789){
          return_int(node->val,root->lineno);
          return node->val;
          }
          else{
          printf("Uninitialized variable in %d\n",root->lineno);
          exit(0);
          }
        } 
        else if(node->typ == TYPE_BOOL) {
          if(node->bool_val != 123456789){
          return_bool(node->bool_val,root->lineno);
          return node->bool_val;
          }
          else{
          printf("Uninitialized variable in %d\n",root->lineno);
          exit(0);
          }
        } 
          else if(node->typ == TYPE_STRING) {
          // The value of an id is a string
          if(node->str_val){
            return_string(node->str_val);
            return node->val;
          }
          else{
          printf("Uninitialized variable in %d\n",root->lineno);
          exit(0);
          }
          }
        }
      
      else {
        //Error! I will exit.
       printf("Uninitialized variable in line %d\n",root->lineno);
        exit(0);
      }
    break;

    case NODE_ASSIGN:
      //We let the RHS expression determine our return value.
      i=interpret(root->right);
      if(is_ret_str) {
        // The result type of rhs is a string. 
        add_key_value_str(root->left->id_val,iret_str);
        //printf("%s",iret_str);
        return 0;
      } 
      else if(is_ret_int){
        // the result type of rhs is an integer contained in iret_int,
        // and also in i.
        add_key_value(root->left->id_val, i);
        //printf("%d",i);
        return i;
      } 
      else if(is_ret_bool){
        add_key_value_bool(root->left->id_val,iret_bool);
        return 0;
      } 
      else {
        printf("Something is wrong with iret functions!\n");
        return 0;
      }
    break;

    case NODE_ADD:
      // We assume that the operands are always integers. So, we simply
      // rely on the return value.
      i = interpret(root->left);
      j = interpret(root->right);
      return_int(i+j,root->lineno);
      return i+j;
    break;

    case NODE_SUB:
      i = interpret(root->left);
      j = interpret(root->right);
      return_int(i-j,root->lineno);
      return i-j;
    break;

    case NODE_LESS:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i<j,root->lineno);
      return i<j;
    break;

    case NODE_LESSEQUAL:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i<=j,root->lineno);
      return i<=j;
    break;
    
    case NODE_GREAT:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i>j,root->lineno);
      return i>j;
    break;
    case NODE_GREATEQUAL:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i>=j,root->lineno);
      return i>=j;
    break;
    case NODE_MUL:
      i = interpret(root->left);
      j = interpret(root->right);
      return_int(i*j,root->lineno);
      return i*j;
    break;

    case NODE_AND:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i && j,root->lineno);
      return i && j;
    break;

    case NODE_OR:
      i = interpret(root->left);
      //printf("%d",i);
      if(is_ret_bool) {
        j = interpret(root->right);
        return_bool(i || j,root->lineno);
        return i || j;
      } 
      else {
        char* str1 = iret_str;
        interpret(root->right);
        char* str2 = iret_str;
        char* str = (char*)malloc(strlen(str1)+strlen(str2)+2);
        strcpy(str,str1);
        return_string(strcat(str,str2));
        return 0;
      }
    break;

    case NODE_COMPARE:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i==j,root->lineno);
      return i==j;
    break;

    case NODE_NOTEQUAL:
      i = interpret(root->left);
      j = interpret(root->right);
      return_bool(i!=j,root->lineno);
      return i!=j;
    break;
    
    case NODE_IF:
      i = interpret(root->left);
      if(i == true){
        is_elsestat=false;
        j=interpret(root->right);
      }
      else {
        is_elsestat=true;
        j=interpret(root->right);
      }
      is_elsestat = false;
      return 0;
    break;

    case NODE_FOR: {
      sum=0;
      j = lookup(root->left->id_val);
      struct sym_node old_node;
      if(j>=0) {
        old_node = *sym_tab[j];
      }
      push_val();
      add_key_value(root->left->id_val, root->forlbound);
      i = root->forlbound;
      while(i <= root->forubound) {
        sum=sum+interpret(root->right);
        i=i+1;
        add_key_value(root->left->id_val,i);
      }
      if(j>=0) {
        sym_tab[j]->key = old_node.key;
        sym_tab[j]->typ = old_node.typ;
        sym_tab[j]->val = old_node.val;
        sym_tab[j]->str_val = old_node.str_val;
        sym_tab[j]->bool_val = old_node.bool_val;
        //add_key_value(root->left->id_val,old_node->val);
      }
      pop_val();
      return_int(sum,root->lineno);
      return sum;
    }
    break;

    case NODE_IFBODY:
      if(is_elsestat == 0){
        interpret(root->left);
      }
      else{
        if(root->right != NULL) {
          interpret(root->right);
        }
      }
      return 0;
    break;

    case NODE_WHILE:
      i=interpret(root->left);
     // interpret(root->right);
      while (i!=0){
        interpret(root->right);
        i = interpret(root->left);       
      }
      return 0;

    break;

    case NODE_PARAMLN:
      // Left child is the param ode. We print/println it.
      interpret(root->left);
      if(is_ret_str) {
        printf("%s ",iret_str);
      } else if(is_ret_int) {
        printf("%d ",iret_int);
      } else {
        if(iret_bool == true){
          printf("true ");
        }
        else{
          printf("false ");
        }
      }
      // If root->right is not null, then there are more params. We
      // print them as well.
      if(root->right != NULL) {
        j = interpret(root->right);
      }
      return 0;
    break;
    
    case NODE_PARAM:
      // Left child is the param ode. We print/println it.
      interpret(root->left);
      if(is_ret_str) {
        printf("%s ",iret_str);
      } else if(is_ret_int) {
        printf("%d ",iret_int);
      } else {
        if(iret_bool == true){
          printf("true ");
        }
        else{
          printf("false ");
        }
      }
      // If root->right is not null, then there are more params. We
      // print them as well.
      if(root->right != NULL) {
        j = interpret(root->right);
      }
      return 0;
    break;

    case NODE_PRINT:
      //interpret root->left. When the interpreter encounters a param
      //node, it prints the param, followed by the given seperator
      //(space)
      i = interpret(root->left);
      return 0;
    break;

    case NODE_PRINTLN:
      //interpret root->left. When the interpreter encounters a param
      //node, it prints the param, followed by the given seperator
      //(newline).
      interpret(root->left);
      printf("\n");
      return 0;
    break;

    case NODE_COMPOUND_STMT:
      push_val();
      //printf("count is %d\n",count);
      interpret(root->left);
      pop_val();
      return 0;
    break;

    case NODE_IDSEQ:
      printf("NODE_IDSEQ reached in interpreter!\n");
      return 0;
    break;

    case NODE_DECL:
      typ  = type_of_decl(root->left);
      declare_id(typ,root->right);
      //printf("%d",i);
      //decl_key(interpret(root->right),typ);
      return 0;
    break; 
    default:
     printf("Invalid input to the interpreter!\n");
    break;
  }
}

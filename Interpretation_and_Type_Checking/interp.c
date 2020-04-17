#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "ast.h"

struct sym_node {
  char* key;
  int val;
  char* str_val;
  
};

#define SYM_TAB_SIZE 1000

struct sym_node* sym_tab[SYM_TAB_SIZE];

static short count = 0;

static int lookup(char* key) {
  for(int i=0; i<count; i++) {
    // get the node at sym_tab[i]
    struct sym_node* node = sym_tab[i];
    if(strcmp(key,node->key) == 0) {
      return i;
    }
  }
  return -1;
}

static void add_key_value(char* key, int val) {
  int index = lookup(key);
  if (index >= 0) {
    // its a valid index
    struct sym_node* node = sym_tab[index];
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
    new_node->str_val = str_val;
    // now, insert the node into the symbol table.
    sym_tab[new_index] = new_node;
  }
  return;
}

bool is_iret_str; //set to true if the return value is a string.
bool is_ret_bool; //set to true if the return value is a boolean
bool is_ret_int; //set to true if the return value is a int
bool iret_bool; //boolean return value.
int iret_int; // the integer return value.
char* iret_str; // the string return value.
const char* print_sep = " "; // seperator between params while printing. Space is the seperator for Print. Newline ("\n") for Println.
bool is_elsestat; //sets the else flag to TRUE if there is any else statement

int interpret(struct ast_node* root){
  //TODO: impement
 if (root == NULL)  {
    return 0;
 }
 int i,j,sum;
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
      is_iret_str = false;
      is_ret_bool = false;
      iret_int = root->int_val;
      // For int values, we also return them explcitly for convenience.
      return root->int_val;
    break;

    case NODE_BOOL:
      is_iret_str = false;
      is_ret_int = false;
      iret_bool = root->bool_val;
      return root->bool_val;
    break;

    case NODE_STRING:
      //return value must be a string
      is_iret_str = true;
      is_ret_bool = false;
      is_ret_int = false;
      iret_str = root->str_val;
      return -1;
    break;

    case NODE_ID:
      i=lookup(root->id_val);
      if (i > -1){
        struct sym_node* node =sym_tab[i];
        if(node->str_val == NULL) {
          // The value of an id is an integer
          is_iret_str = false;
          iret_int = node->val;
          return iret_int;
        } else {
          // The value of an id is a string
          is_iret_str = true;
          iret_str = node->str_val;
          return -1;
        }
      }
      else {
        //Error! I will exit.
       // printf("Uninitialized variable in line %d\n",root->lineno);
        exit(0);
      }
    break;

    case NODE_ASSIGN:
      //We let the RHS expression determine our return value.
      i=interpret(root->right);
      if(is_iret_str) {
        // The result type of rhs is a string. 
        add_key_value_str(root->left->id_val,iret_str);
        is_iret_str = false;
        iret_int = 0;
        return 0;
      } else {
        // the result type of rhs is an integer contained in iret_int,
        // and also in i.
        add_key_value(root->left->id_val, i);
        return i;
      }
    break;

    case NODE_ADD:
      // We assume that the operands are always integers. So, we simply
      // rely on the return value.
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i+j;
      return iret_int;
    break;

    case NODE_SUB:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i-j;
      return iret_int;
    break;

    case NODE_LESS:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i<j;
      return iret_int;
    break;

    case NODE_LESSEQUAL:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i<=j;
      return iret_int;
    break;
    
    case NODE_GREAT:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i>j;
      return iret_int;
    break;
    case NODE_GREATEQUAL:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i>=j;
      return iret_int;
    break;
    case NODE_MUL:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i*j;
      return iret_int;
    break;

    case NODE_AND:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = i && j;
      return iret_int;
    break;

    case NODE_OR:
      i = interpret(root->left);
      j = interpret(root->right);
      if(root->str_val == NULL) {
      is_iret_str = false;
      iret_int = i || j;
      return iret_int;
      } 
      /*else {
      is_iret_str = true;
      return strcat(interpret(root->left),interpret(root->right));
      }*/
    break;

    case NODE_COMPARE:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = (i == j);
      return iret_int;
    break;

    case NODE_NOTEQUAL:
      i = interpret(root->left);
      j = interpret(root->right);
      is_iret_str = false;
      iret_int = (i != j);
      return iret_int;
    break;
    
    case NODE_IF:
      i = interpret(root->left);
      is_iret_str = false;
      if(i == 1){
        is_elsestat=false;
        j=interpret(root->right);
      }
      else {
        is_elsestat=true;
        j=interpret(root->right);
      }
      is_elsestat = false;
      return j;
    break;
    case NODE_FOR:
      sum=0;
      add_key_value(root->left->id_val, root->forlbound);
      i = root->forlbound;
       while(
        //(root->id_val >= root->forlbound) 
        i <= root->forubound){
        sum=sum+interpret(root->right);
        i=i+1;
        add_key_value(root->left->id_val,i);
        
    }
    return sum;

    case NODE_IFBODY:
    
      is_iret_str = false;
      if(is_elsestat == 0){
        return interpret(root->left);
      }
      else{
        if(root->right != NULL) {
        return interpret(root->right);
        }
        else
        {
          return 0;
         }
      }
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

    case NODE_PARAM:
      // Left child is the param ode. We print/println it.
      interpret(root->left);
      if(is_iret_str) {
        printf("%s ",iret_str);
      } else {
        printf("%d ",iret_int);
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
      interpret(root->left);
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
      interpret(root->left);
      return 0;
    break;
    case NODE_IDSEQ:
      interpret(root->left);
      interpret(root->right);
      return 0;
    break;
    case NODE_DECL:
      //Type declaration has no runtime semantics. Do nothing.
      return 0;
 
    default:
     printf("Invalid input to the interpreter!\n");
    break;
  }
}

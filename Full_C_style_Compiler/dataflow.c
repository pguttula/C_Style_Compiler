#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "ast.h"

struct def_node{
  char* id;
  bool is_def;
};

struct err_node{
  char* id;
  int lineno;
};

#define DEF_TAB_SIZE 1000
#define ERR_TAB_SIZE 400
struct def_node* def_tab[DEF_TAB_SIZE];
struct err_node* err_tab[ERR_TAB_SIZE];

static short count = 0;
static int err_count = 0;
static int def_stack[200];
static int def_stack_top=-1;

void def_push(){
  def_stack_top++;
  def_stack[def_stack_top]=count;
  return;
}

void def_pop(){
  if(def_stack_top<0){
    printf("Not supposed to come here");
  }
  else{
    count=def_stack[def_stack_top];
    def_stack_top--;
  }
  return;
}

static int lookup_curr_def(char* id){
  int first = def_stack[def_stack_top];
  for (int i=count-1;i>=first;i--){
    struct def_node* node = def_tab[i];
    if(strcmp(id,node->id) == 0){
      return i;
    }
  }
  return -1;
}

static void add_to_def(struct ast_node* node){
  switch(node->kind) {
  case NODE_ID: {
  char* id=node->id_val;
  int new_index=count;
  count++;
  struct def_node* new_node = (struct def_node*)malloc(sizeof(struct def_node));
  new_node->id = id;
  new_node->is_def = false;
  def_tab[new_index]= new_node;
  //printf("%s,%d now now\n",id,count);
  return;
                }
  break;
  case NODE_IDSEQ:
  add_to_def(node->left);
  add_to_def(node->right);
  return;
  break;
  default:
  printf("Issue with declaration");
  exit;
  break;
  }
}

static void add_assign_def(struct ast_node* node){
  switch(node->kind) {
  case NODE_ID: {
  char* id=node->id_val;
  int new_index=count;
  count++;
  struct def_node* new_node = (struct def_node*)malloc(sizeof(struct def_node));
  new_node->id = id;
  new_node->is_def = true;
  def_tab[new_index]= new_node;
  //printf("%s,%d Input val\n",id,count);
  //if(new_node->is_def ==  true){
  //  printf("1 bool\n");
  //}
  //else{
    //printf("0 bool\n");
  //}
  return;
                }
  break;
  case NODE_IDSEQ:
  add_assign_def(node->left);
  add_assign_def(node->right);
  return;
  break;
  default:
  printf("Issue with assignment/input");
  exit;
  break;
  }
}

void error(char* id,int lineno){
  int new_ind = err_count;
  err_count++;
  struct err_node* new_node = (struct err_node*)malloc(sizeof(struct err_node*));
  new_node->id = id;
  new_node->lineno = lineno;
  err_tab[new_ind] = new_node;
  return;
}

void print_errors(){
  for(int i=0;i<err_count;i++){
    struct err_node* node=err_tab[i];
    printf("%s in %d may be uninitialized\n",node->id,node->lineno);
  }
  return;
}

static int lookup_in_def(char* id,int lineno){
  int j;
  for(int i=count-1;i>=0;i--){
    struct def_node* node = def_tab[i];
    if(strcmp(id,node->id) == 0){
      //printf("def table val%s,%d\n",node->id,i);
        j=i;
        struct def_node* new_node = def_tab[j];
        if(new_node->is_def == true){
          return j;
        }
        else{
    //            printf("%s,%d LOOK \n",new_node->id,count);
          return -1;
        }
      }
    }
  }
  
//TODO:to sort and print errors in err_tab
//

/*
 * Returns variables that are initialized (not (re)declared) in top
 * frame.
 */
int get_vars_in_top_frame(char* vars[]) {
  int i,j,k=0;
  int sf_begin = def_stack[def_stack_top];
  int sf_end = count - 1;
  bool is_redef[50];
  for(i=sf_begin; i<=sf_end; i++) {
    if(!def_tab[i]->is_def) {
      for(j=i+1;j<=sf_end;j++) {
        if(strcmp(def_tab[j]->id, def_tab[i]->id) == 0 
            && def_tab[j]->is_def) {
          def_tab[j]->is_def = false;
        }
      }
    }
    else {
      vars[k] = def_tab[i]->id;
      k++;
    }
  }
  return k;
}

// common vars are filled in vars1
int get_common_vars(char* vars1[], int size1, 
                    char* vars2[], int size2) {
  int i,j,k=0;
  char* cvars[50];
  for(i=0;i<size1;i++) {
    for(j=0;j<size2;j++) {
      if(strcmp(vars1[i],vars2[j]) == 0){
        cvars[k] = vars1[i];
        k++;
      }
    }
  }

  for(i=0;i<k;i++) {
    vars1[i] = cvars[i];
  }
  return k;
}

void data_flow(struct ast_node* root){

  int i,j;
  if (root == NULL)  {
    printf("Dataflow Analysis happenning on NULL");
    exit(0);
  }

  switch(root->kind) {
    case NODE_DECL:
    add_to_def(root->right);
    break;

    case NODE_INT:
    return;
    break;

    case NODE_STRING:
    return;
    break;

    case NODE_ASSIGN:
    data_flow(root->right);
    add_assign_def(root->left);
    break;

    case NODE_ID:
    i = lookup_in_def(root->id_val,root->lineno);
    //printf("%d,%s ID\n",i,root->id_val);
    if(i<0){
    //printf("ID in %d may be uninitialized",root->lineno);
    error(root->id_val,root->lineno);
    }
    break;

    case NODE_INPUT:
    add_assign_def(root->left); 
    break;
    
    case NODE_IF:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_IFBODY:
    if(root->right == NULL) {
      def_push();
      data_flow(root->left);
      def_pop();
    }
    else {
      def_push();
      data_flow(root->left);
      char* vars1[50];
      int cnt1 = get_vars_in_top_frame(vars1);
      //printf("cnt1 is %d\n",cnt1);
      def_pop();

      def_push();
      data_flow(root->right);
      char* vars2[50];
      int cnt2 = get_vars_in_top_frame(vars2);
      //printf("cnt2 is %d\n",cnt2);
      def_pop();

      int cnt = get_common_vars(vars1,cnt1,vars2,cnt2);
      for(i=0;i<cnt;i++) {
        struct ast_node* id_node = mk_id_node(vars1[i]);
        add_assign_def(id_node);
        free(id_node);
      }
    }
    break;
    
    case NODE_WHILE:
    def_push();
    data_flow(root->left);
    data_flow(root->right);
    def_pop();
    break;

    case NODE_FOR:
    def_push();
    add_assign_def(root->left);
    data_flow(root->right);
    def_pop();
    break;
    
    case NODE_BOOL:
    return;
    break;
    case NODE_PARAM:
    data_flow(root->left);
    if(root->right != NULL) {
      data_flow(root->right);
    }
    break;

    case NODE_PARAMLN:
    data_flow(root->left);
    if(root->right != NULL) {
      data_flow(root->right);
    }
    break;

    case NODE_PRINT:
    data_flow(root->left);
    break;

    case NODE_PRINTLN:
    data_flow(root->left);
    break;

    case NODE_COMPOUND_STMT: {
      def_push();
      char* vars[50];
      data_flow(root->left);
      int cnt = get_vars_in_top_frame(vars);
      //printf("cnt is %d\n",cnt);
      def_pop();
      for(i=0;i<cnt;i++) {
        struct ast_node* id_node = mk_id_node(vars[i]);
        add_assign_def(id_node);
        free(id_node);
      }
    }
    break;

    case NODE_SEQ:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_ADD:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_SUB:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_MUL:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_LESS:
    data_flow(root->left);
    data_flow(root->right);
    break;
      
    case NODE_GREAT:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_LESSEQUAL:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_GREATEQUAL:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_COMPARE:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_NOTEQUAL:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_AND:
    data_flow(root->left);
    data_flow(root->right);
    break;

    case NODE_OR:
    data_flow(root->left);
    data_flow(root->right);
    break;

    default:
    printf("Data Flow Analysis is not working as expected");
    exit;
    break;
  }
    
return;
}
void data_flow_stmt(struct ast_node* prog) {
  data_flow(prog);
  print_errors();
}

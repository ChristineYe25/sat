#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
//#include <string> 
using namespace std; 

#define NUM_CLAUSES 1065
#define NUM_VARS 250 
#define BUF_SIZE 5

#define F 2
#define T 1
#define Undef 0

#define SOLVED 0
#define DECISION 1
#define PROP 2
#define ANAYLSIS 3
#define BACKTRACK 4 
#define FAILED 5


void collect_buffer(int pos_cls[NUM_VARS][BUF_SIZE], int neg_cls[NUM_VARS][BUF_SIZE], 
  const int var, const int x){
   if (var> 0){
      //assert(pos_cls[var][4]>0);
      if (pos_cls[var][0] == 0){
        pos_cls[var][0] = x; 
      }else if (pos_cls[var][1] == 0){
        pos_cls[var][1] = x; 
      }else if (pos_cls[var][2] == 0){
        pos_cls[var][2] = x; 
      }else if (pos_cls[var][3] == 0){
        pos_cls[var][3] = x; 
      }else {
        pos_cls[var][4] = x; 
        printf("Get here "); 
      }
    }else{
      //assert(neg_cls[var][4]>0);
      if (neg_cls[-var][0] == 0){
        neg_cls[-var][0] = x; 
      }else if (neg_cls[-var][1] == 0){
        neg_cls[-var][1] = x; 
      }else if (neg_cls[-var][2] == 0){
        neg_cls[-var][2] = x; 
      }else if (neg_cls[-var][3] == 0){
        neg_cls[-var][3] = x; 
      }else { 
        neg_cls[-var][4] = x; 
        printf("Get here 2"); 
      }
    }
}


void deduction(int l1, int l2, int *var_truth_table, bool conflict, int *l_ded){
  if (l1>0 && l2>0){
    // F & F or F -> T or T <- F
    if (var_truth_table[l1] == F){
      if (var_truth_table[l2] == F){
        conflict = 1;
      }else {
        l_ded[1] = T;
        conflict = 0;
      }
    }else if (var_truth_table[l1] == Undef && var_truth_table[l2] == F ){
      l_ded[0] = T;
    }
  }else if (l1>0 && l2<0){
    // F & F or F -> ~F or T <- ~T
    if (var_truth_table[l1] == F){
      if (var_truth_table[-l2] == T){
        conflict = 1;
      }else {
        l_ded[1] = F;
      }
    }else if (var_truth_table[l1] == Undef && var_truth_table[l2] == T){
      l_ded[0] = T;
    }
  }else if (l1<0 && l2>0){
    // ~T & F or ~F -> T or ~T <- F
    if (var_truth_table[-l1] == T){
      if (var_truth_table[l2] == F){
        conflict = 1;
      }else {
        l_ded[1] = T;
      }
    }else if (var_truth_table[l1] == Undef && var_truth_table[l2] == F){
      l_ded[0] = T;
    }
  }else{
    // ~T & ~T or ~T -> ~F or ~F <- ~T
    if (var_truth_table[-l1] == T){
      if (var_truth_table[-l2] == T){
        conflict = 1;
      }else {
        l_ded[1] = F;
      }
    }else if (var_truth_table[l1] == Undef && var_truth_table[l2] == F){
      l_ded[0] = F;
    }
  }

}


#pragma ACCEL kernel
void solver_kernel(
        int* c1, int* c2, int* c3, //int* num_pos_cls, int* num_neg_cls, 
        int* result){
   //     int* pos_cls, int* neg_cls, int* pos_cls_idx, int* neg_cls_idx) {

#pragma ACCEL interface variable=c1 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c2 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c3 bus_bitwidth=512 depth = 1065
//#pragma ACCEL interface variable=num_pos_cls bus_bitwidth=512 depth = 1065
//#pragma ACCEL interface variable=num_neg_cls bus_bitwidth=512 depth = 1065  

  /*
#pragma ACCEL interface variable=pos_cls bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=neg_cls bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=pos_cls bus_bitwidth=512 depth = 250
#pragma ACCEL interface variable=neg_cls bus_bitwidth=512 depth = 250 
*/
#pragma ACCEL interface variable=result depth=1 
 
  int satisfiable; 
  int local_clauses[NUM_CLAUSES][3];
  int pos_cls[NUM_VARS][BUF_SIZE] = {0};
  int neg_cls[NUM_VARS][BUF_SIZE] = {0};
  //int **pos_cls = (int **)malloc(NUM_VARS * sizeof(int *)); 
  //int **neg_cls = (int **)malloc(NUM_VARS * sizeof(int *)); 

  int state = 0; 
 
  int assigned_vars_stack[NUM_VARS]; // assigend value stack
  bool assigned_status[NUM_VARS]; //1: if we assigned both True and False to variables 
  int stack_end_ptr = -1; 
  int var_truth_table[NUM_VARS];

/*
  for (int i=0; i<NUM_VARS; i++){
    pos_cls[i] = (int *)malloc(num_pos_cls[i] * sizeof(int));
    neg_cls[i] = (int *)malloc(num_neg_cls[i] * sizeof(int));
  }
*/

  //#pragma ACCEL pipeline 
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    local_clauses[x][0] = c1[x];
    local_clauses[x][1] = c2[x];
    local_clauses[x][2] = c3[x];
    
    collect_buffer(pos_cls, neg_cls, c1[x], x);
    collect_buffer(pos_cls, neg_cls, c2[x], x);
    collect_buffer(pos_cls, neg_cls, c3[x], x);
  }

  printf("Finish reading dsata \n");
  int new_var_idx = 0; 
  while (~(state == SOLVED | state == FAILED)){

    switch(state){
      case DECISION:
        if (new_var_idx == NUM_VARS)
          state = SOLVED;

        while (var_truth_table[new_var_idx] != Undef){
          new_var_idx ++; 
        }
        
        
        var_truth_table[new_var_idx] = T; //always assigned to True
        stack_end_ptr ++; 
        assigned_vars_stack[stack_end_ptr] = new_var_idx;

        state = PROP;
        printf("Decide var : v");

      case PROP:
        bool conflict[BUF_SIZE];
        //int ded_var_idx[BUF_SIZE];
        //int ded_var_value[BUF_SIZE]; 

        #pragma ACCEL parallel 
        for (int x = 0; x < BUF_SIZE; x++){
          int l1, l2;
          if(var_truth_table[new_var_idx] == T){
            if (neg_cls[new_var_idx][x] == 0){
              conflict[x] = 0; 
            }else{
              if (local_clauses[neg_cls[new_var_idx][x]][0] == new_var_idx){
                l1 = local_clauses[neg_cls[new_var_idx][x]][1];
                l2 = local_clauses[neg_cls[new_var_idx][x]][2];
              }else if (local_clauses[neg_cls[new_var_idx][x]][1] == new_var_idx){
                l1 = local_clauses[neg_cls[new_var_idx][x]][0];
                l2 = local_clauses[neg_cls[new_var_idx][x]][2];
              }else{
                l1 = local_clauses[neg_cls[new_var_idx][x]][0];
                l2 = local_clauses[neg_cls[new_var_idx][x]][1];
              }
              int l_ded[2]; 
              deduction(l1, l2, var_truth_table, conflict[x], l_ded);
            }
          }else{
            if (pos_cls[new_var_idx][x] == 0){
              conflict[x] = 0; 
            }else{
              if (local_clauses[pos_cls[new_var_idx][x]][0] == new_var_idx){
                l1 = local_clauses[pos_cls[new_var_idx][x]][1];
                l2 = local_clauses[pos_cls[new_var_idx][x]][2];
              }else if (local_clauses[pos_cls[new_var_idx][x]][1] == new_var_idx){
                l1 = local_clauses[pos_cls[new_var_idx][x]][0];
                l2 = local_clauses[pos_cls[new_var_idx][x]][2];
              }else{
                l1 = local_clauses[pos_cls[new_var_idx][x]][0];
                l2 = local_clauses[pos_cls[new_var_idx][x]][1];
              }
              int l_ded[2]; 
              deduction(l1, l2, var_truth_table, conflict[x], l_ded);
            }
          }
        }

        if (conflict[0] | conflict[1] | conflict[2] | conflict[3] | conflict[4]){
          //Found conflict, go back decision
          state = BACKTRACK;
        }else{      
          state = DECISION;   
          new_var_idx ++; 
        }

      case ANAYLSIS: 

      case BACKTRACK:
        while(assigned_status[stack_end_ptr] == 1){
          //We checked both True and False cases, we need to go back 
          var_truth_table[new_var_idx] = Undef; 
          assigned_vars_stack[stack_end_ptr] = 0; //pop current variables
          stack_end_ptr --; 
          if (stack_end_ptr < 0){
            break; 
            state = FAILED; 
          }
          new_var_idx = assigned_vars_stack[stack_end_ptr]; 
        }

        if (var_truth_table[new_var_idx] == T){
          var_truth_table[new_var_idx] = F;
        }else{
          var_truth_table[new_var_idx] = T;
        }
        assigned_status[stack_end_ptr] = 1; 
        state = PROP; 

      case SOLVED:
        satisfiable = 1; 
      case FAILED: 
        satisfiable = 0; 
    }  

    //propogate
  }

  result[0] = satisfiable; 

}

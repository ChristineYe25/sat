#include <string.h>
#include <math.h>
#include <assert.h>

#define NUM_CLAUSES 1065
#define NUM_VARS 250 
#define BUF_SIZE 5

#define F 2
#define T 1
#define Undef 0

//using std::vector;
void collect_buffer(int pos_cls[NUM_VARS][BUF_SIZE], int neg_cls[NUM_VARS][BUF_SIZE], 
  const int var, const int x){
   if (var> 0){
      if (pos_cls[var][0] == 0){
        pos_cls[var][0] = x; 
      }else if (pos_cls[var][1] == 0){
        pos_cls[var][1] = x; 
      }else if (pos_cls[var][2] == 0){
        pos_cls[var][2] = x; 
      }else if (pos_cls[var][3] == 0){
        pos_cls[var][3] = x; 
      }else if (pos_cls[var][4] == 0){
        pos_cls[var][4] = x; 
      }
    }else{
      if (neg_cls[-var][0] == 0){
        neg_cls[-var][0] = x; 
      }else if (neg_cls[-var][1] == 0){
        neg_cls[-var][1] = x; 
      }else if (neg_cls[-var][2] == 0){
        neg_cls[-var][2] = x; 
      }else if (neg_cls[-var][3] == 0){
        neg_cls[-var][3] = x; 
      }else if (neg_cls[-var][4] == 0){
        neg_cls[-var][4] = x; 
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
        int* c1, int* c2, int* c3, 
        int* result){
   //     int* pos_cls, int* neg_cls, int* pos_cls_idx, int* neg_cls_idx) {

#pragma ACCEL interface variable=c1 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c2 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c3 bus_bitwidth=512 depth = 1065
  /*
#pragma ACCEL interface variable=pos_cls bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=neg_cls bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=pos_cls bus_bitwidth=512 depth = 250
#pragma ACCEL interface variable=neg_cls bus_bitwidth=512 depth = 250 
*/
#pragma ACCEL interface variable=result depth=1 
 
 
  int local_clauses[NUM_CLAUSES][3];
  int pos_cls[NUM_VARS][BUF_SIZE];
  int neg_cls[NUM_VARS][BUF_SIZE];

  int *test = (int *)malloc(sizeof(int) * 10);
  
 // vector<int> local_pos_lit_cls[NUM_VAR];


  //int cl_truth_table[NUM_CLAUSES][3];
  int num_T_cls = 0;  
  //int assigned_var_buf[NUM_VARS][2]; // idx, assigned value 
  int var_truth_table[NUM_VARS];

  //#pragma ACCEL pipeline 
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    local_clauses[x][0] = c1[x];
    local_clauses[x][1] = c2[x];
    local_clauses[x][2] = c3[x];
    collect_buffer(pos_cls, neg_cls, c1[x], x);
    collect_buffer(pos_cls, neg_cls, c2[x], x);
    collect_buffer(pos_cls, neg_cls, c3[x], x);
  }

  int new_var_idx = 0; 
  while (num_T_cls == NUM_CLAUSES){
    num_T_cls ++; 

    //Step 1: decide
    while (var_truth_table[new_var_idx] != 0){
      new_var_idx ++; 
    }
    var_truth_table[new_var_idx] = 1; //assigned to truth

    //propogate
    bool conflict[BUF_SIZE];
    int ded_num[BUF_SIZE][2]; // idx and its value

    #pragma ACCEL parallel 
    for (int x = 0; x < BUF_SIZE; x++){
      int l1, l2;
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
        deduction(l1, l2, var_truth_table, conflict, l_ded);
        if (l_ded[0]!= Undef){
          ded_num[x][0]=l1;
          ded_num[x][1]=l_ded[0]; 
        }else if (l_ded[1] != Undef){
          ded_num[x][0]=l2;
          ded_num[x][1]=l_ded[1];
        }
      }

    }



    if (conflict[0] | conflict[1] | conflict[2] | conflict[3] | conflict[4]){
      //Found conflict, go back to decision
    }else{      
      
    }
    new_var_idx ++; 
  }

  result[0] = new_var_idx; 

}

#include <string.h>
#include <math.h>
#include <assert.h>

#define NUM_CLAUSES 1065
#define NUM_VARS 250 
//using std::vector;
void collect_buffer(int pos_cls[NUM_VARS][5], int neg_cls[NUM_VARS][5], int var){
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
  int pos_cls[NUM_VARS][5];
  int neg_cls[NUM_VARS][5];
  
 // vector<int> local_pos_lit_cls[NUM_VAR];


  //int cl_truth_table[NUM_CLAUSES][3];
  int num_T_cls = 0;  
  int assigned_var_buf[NUM_VARS][2]; // idx, assigned value 
  int var_truth_table[NUM_VARS]; 

  //#pragma ACCEL pipeline 
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    local_clauses[x][0] = c1[x];
    local_clauses[x][1] = c2[x];
    local_clauses[x][2] = c3[x];
    collect_buffer(pos_cls, neg_cls, c1[x]);
    collect_buffer(pos_cls, neg_cls, c2[x]);
    collect_buffer(pos_cls, neg_cls, c3[x]);
  }

  new_var_idx = 0; 
  while (num_T_cls == NUM_CLAUSES){
    num_T_cls ++; 

    //Step 1: decide
    while (var_truth_table[new_var_idx] != 0){
      new_var_idx ++; 
    }
    var_truth_table[new_var_idx] = 1; //assigned to truth

    //propogate
    bool conflict[5];

    #pragma ACCEL parallel 
    for (int x = 0; x < 5; x++){
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
  
        if (l1>0 && l2>0){
          conflict[x] = (var_truth_table[l1] == 2) && (var_truth_table[l2] == 1);
        }else if (l1>0 && L2<0){
          conflict[x] = (var_truth_table[l1] == 2) && (var_truth_table[-l2] == 2);
        }else if (l1<0 && L2>0){
          conflict[x] = (var_truth_table[-l1] != 1) && (var_truth_table[l2] != 1);
        }else{
          conflict[x] = (var_truth_table[-l1] != 1) && (var_truth_table[-l2] != 1);
        }
      }
    }

    new_var_idx ++; 

/*
    if (conflict[0] | conflict[1] | conflict[2] | conflict[3] | conflict[4]){

    }
*/
  }

  result[0] = 3; 

}

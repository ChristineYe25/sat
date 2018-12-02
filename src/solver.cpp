#include <string.h>
#include <math.h>
#include <assert.h>

#define NUM_CLAUSES 1065
#define NUM_VAR 250 

#pragma ACCEL kernel
void solver_kernel(
        int* c1, int* c2, int* c3, 
        //int* pos_lit, int* neg_lit, //const int num_pos, const int num_neg, 
        int* result) {

#pragma ACCEL interface variable=c1 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c2 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c3 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=result depth=1 
 
 
  int c1_local[NUM_CLAUSES];
  int c2_local[NUM_CLAUSES];
  int c3_local[NUM_CLAUSES];
    
  // buffer for cls is 5 for now
  vector<int> pos_cls_idx[NUM_VAR]; 
  vector<int> neg_cls_idx[NUM_VAR]; 

  //int pos_lit_cls[NUM_CLAUSES*3];
  //int neg_lit_cls[NUM_CLAUSES*3];

  //int cl_truth_table[NUM_CLAUSES][3];
  int num_T_cls = 0;  
  int assigned_var_buf[NUM_VAR][2]; // idx, assigned value 
  int var_truth_table[NUM_VAR]; 

  //#pragma ACCEL pipeline 
  
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    c1_local[x] = c1[x]; 
    c2_local[x] = c2[x]; 
    c3_local[x] = c3[x];
    if (c1[x] < 0){
      neg_cls_idx[c1[x]].push(x);
    }else{
      pos_cls_idx[c1[x]].push(x);
    }
  }

  for (int x = 0; x < NUM_VAR; x++){
    var_truth_table[x] = 0;
  }

  idx = 0; 
  while (num_T_cls == NUM_CLAUSES){
    num_T_cls ++; 

    //Step 1: decide
/*
    while (var_truth_table[idx] != 0){
      idx ++; 
    }

    var_truth_table[idx] = 1;

    //propogate
    for (int i =0; i < NUM_CLAUSES; i++){
      int c1_idx = c1_local[i];
      int c2_idx = c2_local[i];
      int c3_idx = c3_local[i];
      if ()
    }
*/


  }
    

/*
  #pragma ACCEL parallel factor = 512
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    tmp[x] = c1_local[x] + c2_local[x] +c3_local[x];
  }
*/

  result[0] =tmp[0]; 

}

#include <string.h>
#include <math.h>
#include <assert.h>

#define NUM_CLAUSES 1065

#pragma ACCEL kernel
void solver_kernel(
        int* c1,
        int* c2,
        int* c3, 
        int* result) {

#pragma ACCEL interface variable=c1 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c2 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=c3 bus_bitwidth=512 depth = 1065
#pragma ACCEL interface variable=result depth=1 
  
  int c1_local[NUM_CLAUSES];
  int c2_local[NUM_CLAUSES];
  int c3_local[NUM_CLAUSES];
  int tmp[NUM_CLAUSES];

  //#pragma ACCEL pipeline 
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    c1_local[x] = c1[x]; 
    c2_local[x] = c2[x]; 
    c3_local[x] = c3[x]; 
  }

  #pragma ACCEL parallel factor = 512
  for (int x = 0; x < NUM_CLAUSES; ++x) {
    tmp[x] = c1_local[x] + c2_local[x] +c3_local[x];
  }

  /*
  while(){

  }*/

  result[0] =tmp[0]; 

}

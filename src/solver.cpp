#include <string.h>
#include <math.h>
#include <assert.h>

#define NUM_CLAUSES 1065

#pragma ACCEL kernel
void solver_kernel(
        int* c1,
        int* c2,
        int* c3, 
        int result) {

#pragma ACCEL interface variable=c1 depth=1065
#pragma ACCEL interface variable=c2 depth=1065
#pragma ACCEL interface variable=c3 depth=1065 
#pragma ACCEL interface variable=result 
  //result = c1[0] + c2[0] +c3[0]; 
  result = 2; 

/*
    unsigned char buf_knn_mat[10][3];

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 3; ++y) {
            buf_knn_mat[x][y] = 50;
        }
    }

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 1800; ++y) {
            int temp = train_images[x * 1800 + y] ^ test_image;
            unsigned char dis = 0;
            for (int i = 0; i < 49; ++i) {
                dis += (temp & (1L << i)) >> i;
            }

            unsigned char max_id = 0;
            unsigned char max_val = 0;
            for (int i1 = 0; i1 < 3; ++i1) {
                if (max_val < buf_knn_mat[x][i1]) {
                    max_id = i1;
                    max_val = buf_knn_mat[x][i1];
                }
            }
            if (dis < max_val) {
                buf_knn_mat[x][max_id] = dis;
            }
        }
    }

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 3; ++y) {
            knn_mat[x * 3 + y] = buf_knn_mat[x][y];
        }
    }
    */
}

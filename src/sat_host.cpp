#include <iostream>
#include <string>

#define NUM_TEST 1
#define NUM_CLAUSES 1065
#define NUM_VAR 250 

using std::cout;
using std::endl;
using std::string;
using std::to_string;

#ifdef MCC_ACC
#include MCC_ACC_H_FILE
#else
void digitrec_kernel(int* c1, int* c2, int* c3, int* result);
#endif

// Util functions for host
void read_clause_file(string filename, int *c1, int *c2, int *c3, 
    int *pos_cls, int *neg_cls, 
    int *pos_cls_idx, int *neg_cls_idx);


int main(int argc, char **argv) {

  int *c1 = (int *)malloc(sizeof(int) * NUM_CLAUSES);
  int *c2 = (int *)malloc(sizeof(int) * NUM_CLAUSES);
  int *c3 = (int *)malloc(sizeof(int) * NUM_CLAUSES);
  int *pos_cls = (int *)malloc(sizeof(int) * NUM_CLAUSES); 
  int *neg_cls = (int *)malloc(sizeof(int) * NUM_CLAUSES); 
  int *pos_cls_idx = (int *)malloc(sizeof(int) * NUM_VAR); 
  int *neg_cls_idx = (int *)malloc(sizeof(int) * NUM_VAR); 
 
  int *result = (int *)malloc(sizeof(int));

  if (argc < 2) {
    cout << "Usage: ./a.out <data path>" << endl;
    return 0;
  }

#ifdef MCC_ACC
  __merlin_init(argv[argc-1]);
#endif

  //string path(argv[1]);

  // Prepare data
  //for (int i = 0; i < NUM_TEST; ++i)
  //  read_clause_file("./data/uf250-01.cnf", c1, c2, c3);

  cout << "Clause :"<< c1[0] << " " << c2[0]<< " " <<c3[0] << endl; 
  // Compute
  for (int i = 0; i < NUM_TEST; ++i) {
    read_clause_file("./data/uf250-01.cnf", c1, c2, c3); 
    //  pos_cls, neg_cls, pos_cls_idx, neg_cls_idx);
#ifdef MCC_ACC
    __merlin_solver_kernel(c1, c2, c3, result); 
     // pos_cls, neg_cls, pos_cls_idx, neg_cls_idx);); 
#else
    solver_kernel(c1, c2, c3, result);
     // pos_cls, neg_cls, pos_cls_idx, neg_cls_idx););
#endif

    cout << "Result : " << result[0] << endl; 
  }

#ifdef MCC_ACC
    __merlin_release();
#endif

  free(c1);
  free(c2);
  free(c3);
  return 0;
}

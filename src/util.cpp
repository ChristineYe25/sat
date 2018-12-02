#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;
using std::sort;
using std::istringstream;

#define NUM_CLAUSES 1065
#define NUM_VAR 250
void read_clause_file(string filename, int *c1, int *c2, int *c3){ 
  //  int *pos_cls, int *neg_cls, 
  //  int *pos_cls_idx, int *neg_cls_idx){

  ifstream f;
  int l1, l2, l3; 
  vector<int> local_pos_lit_cls[NUM_VAR];
  vector<int> local_neg_lit_cls[NUM_VAR];

  f.open(filename.c_str(), ios::in);
  if (!f.is_open()) {
    cout << "Open " << filename << " failed" << endl;
    exit(1);
  }

  int cnt = 0;
  cout << "Start to read file" <<endl; 
  string line;
  while (std::getline(f, line)) {
    if (line == "")
      continue;
    
    if (line.at(0) != 'p' and line.at(0) != 'c') {
      vector<string> substrs;
      istringstream iss(line);
      for(string s; iss >> s;)
        substrs.push_back(s);

      if (substrs.size() < 2)
        continue;

      l1 = stoi(substrs.at(0));
      l2 = stoi(substrs.at(1));
      l3 = stoi(substrs.at(2));
      c1[cnt] = l1;
      c2[cnt] = l2;
      c3[cnt] = l3;
      /*
      if (l1 > 0){
        local_pos_lit_cls[l1].push(cnt);
      }else{
        local_neg_lit_cls[-l1].push(cnt);
      }
      if (l2 > 0){
        local_pos_lit_cls[l2].push(cnt);
      }else{
        local_neg_lit_cls[-l2].push(cnt);
      }
      if (l3 > 0){
        local_pos_lit_cls[l3].push(cnt);
      }else{
        local_neg_lit_cls[-l3].push(cnt);
      }*/
      //cout << "Clause :"<< c1[cnt] << " " << c2[cnt]<< " " <<c3[cnt] << "\n"; 
      cnt ++; 
    }
  }
  cout << "Number of clauses : " << cnt << endl << "Finish reading file" << endl;
  /*
  int x = 0; 
  for (int i =0; i<NUM_VAR ; i++){
    pos_cls_idx[i] = x; 
    for (int j = 0; j < local_pos_lit_cls[i].size(); j++){
      pos_cls[x] = local_pos_lit_cls[j];
      x++; 
    }
  }*/

  f.close();
  return ;
}
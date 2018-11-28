#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "xcl2.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;
using std::sort;

void read_clause_file(string filename,
    vector<int, aligned_allocator<int>> &c1,
    vector<int, aligned_allocator<int>> &c2,
    vector<int, aligned_allocator<int>> &c3) { 

  ifstream f;
  f.open(filename.c_str(), ios::in);
  if (!f.is_open()) {
    cout << "Open " << filename << " failed" << endl;
    exit(1);
  }

  int cnt = 0;
  while (!f.eof()) {
    string str;
    f >> str;
    if str.at(0) != 'p' and str.at(0) != 'c' {
      substrs = strtok(str, " ");
      if substrs[0] == ""{
        c1[cnt] = atoi(substrs[1]);
        c2[cnt] = atoi(substrs[2]);
        c3[cnt] = atoi(substrs[3]);
      }else{
        c1[cnt] = atoi(substrs[0]);
        c2[cnt] = atoi(substrs[1]);
        c3[cnt] = atoi(substrs[2]);
      }
      cnt ++; 
    }
  }

  f.close();
  return ;
}
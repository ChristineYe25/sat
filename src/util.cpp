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

void read_clause_file(string filename, int *c1, int *c2, int *c3){

  ifstream f;
  f.open(filename.c_str(), ios::in);
  if (!f.is_open()) {
    cout << "Open " << filename << " failed" << endl;
    exit(1);
  }

  int cnt = 0;
  cout << "Start to read file"; 
  while (!f.eof()) {
    string line;
    f >> line; 
    if (line.at(0) != 'p' and line.at(0) != 'c') {
      vector<string> substrs;
      istringstream iss(line);
      for(string s; iss >> s; )
        substrs.push_back(s);

      if (substrs.size() < 2)
        continue;
      cout << "Clause :"<< substrs.at(0) << " " << substrs.at(1) << "\n"
      c1[cnt] = stoi(substrs.at(0));
      c2[cnt] = stoi(substrs.at(1));
      c3[cnt] = stoi(substrs.at(2));
      cnt ++; 
    }
  }
  cout << "Final cnt is " << cnt;

  f.close();
  return ;
}
#ifndef BASICS_H
#define BASICS_H

#include <unordered_set>
#include <set>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class Basics {
private:
  vector <set <int>> neighbors;
public:
  bool check_PIDS(unordered_set <int> subset);
  bool check_MPIDS(unordered_set <int> subset);
  void setNeihbors (vector <set <int> > n);
};
#endif

#include <unordered_set>
#include <set>
#include <vector>
#include <iostream>
using namespace std;

vector<set<int> > neighbors;

bool check_PIDS(unordered_set <int> subset) {
    for (set <int> s : neighbors){
        int count = 0;
        for (int i : s) {
            if (subset.find(i) != subset.end())
                count++;
        }
        if (count < s.size()/2.f) return false;
    }
    return true;
}

bool check_MPIDS(unordered_set <int> subset) {
    if (!check_PIDS(subset)) return false;
    unordered_set<int> aux = subset;
    for (int s : subset) {
        aux.erase(s);
        if (check_PIDS(aux)) return false;
        aux.insert(s);
    }
    return true;
}

void setNeighbors (vector <set <int> > n) {
  neighbors = n;
}

int main () {
  neighbors = vector<set<int> >(10);
  neighbors[0] = {5, 7};
  neighbors[1] = {2};
  neighbors[2] = {1, 6, 8, 9};
  neighbors[3] = {6};
  neighbors[4] = {6};
  neighbors[5] = {0, 7, 9};
  neighbors[6] = {2, 3, 4};
  neighbors[7] = {0, 5};
  neighbors[8] = {2};
  neighbors[9] = {2, 5};


  /*neighbors[0] = {1, 2};
  neighbors[1] = {0, 2};
  neighbors[2] = {0, 1, 3, 4};
  neighbors[3] = {2};
  neighbors[4] = {2, 7, 5};
  neighbors[5] = {4, 6};
  neighbors[6] = {5};
  neighbors[7] = {4};*/
  unordered_set<int> ss = {4, 6, 2, 9, 5, 0};
}

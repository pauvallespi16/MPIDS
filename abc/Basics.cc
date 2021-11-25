#include "Basics.h"
#include <unordered_set>
#include <set>
#include <iostream>
using namespace std;

bool Basics::check_PIDS(unordered_set <int> subset) {
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

bool Basics::check_MPIDS(unordered_set <int> subset) {
    if (!check_PIDS(subset)) return false;
    unordered_set<int> aux = subset;
    for (int s : subset) {
        aux.erase(s);
        if (check_PIDS(aux)) return false;
        aux.insert(s);
    }
    return true;
}

void Basics::setNeighbors (vector <set <int> > n) {
  neighbors = n;
}

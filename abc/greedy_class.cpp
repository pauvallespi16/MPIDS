#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Timer.h"
#include "Random.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <set>
#include <cmath>
#include <unordered_set>
#include <limits>
#include <iomanip>

// Data structures for the problem data
vector< unordered_set<int> > neighbor;
vector<int> neighbor_popularity;

// C++ program to find the Dominant Set of a graph
using namespace std;


bool check_PIDS(unordered_set <int> subset) {
    for (auto s : neighbor){
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

bool check_adjacent_neighbor(const unordered_set<int>& node_neighbor) {
    for (int node : node_neighbor) {
        if (neighbor_popularity[node] < neighbor[node].size()/2.f) {
            for (int neighbor : node_neighbor)
                neighbor_popularity[neighbor]++;
            return true;
        }
    }
    return false;
}

/*
//Cambiar subset a vector para guardar último nodo visitado y pasarlo por parámetro
int find_removable_node(unordered_set <int> subset) {
    unordered_set<int> aux = subset;
    for (int s : subset) {
        aux.erase(s);
        for (int n : neighbor[s]) {
            neighbor_popularity[n]--;
        }

        if (!check_adjacent_neighbor(neighbor[s])) return s;
        aux.insert(s);
    }
    return -1;
}*/


bool compare(int i, int j) {
    return neighbor[i].size() > neighbor[j].size();
}

unordered_set<int> greedy() {
    unordered_set<int> solution;
    neighbor_popularity = vector<int>(neighbor.size(), 0);
    vector<int> index_array(neighbor.size());

    for (int i = 0; i < neighbor.size(); i++) index_array[i] = i;
    sort (index_array.begin(), index_array.end(), compare);

    int pos = neighbor.size()-1;
    while (pos >= 0 and neighbor[index_array[pos]].size() == 0) --pos;
    while (pos >= 0 and neighbor[index_array[pos]].size() == 1) {
        auto it = neighbor[index_array[pos]].begin();
        if (solution.find(*it) == solution.end()) {
            solution.insert(*it);
            for (int neighbor : neighbor[*it]) {
                neighbor_popularity[neighbor]++;
            }
        }
        --pos;
    }

    for (int top = 0; top < neighbor.size() ; top++) {
        if (solution.find(index_array[top]) == solution.end()) {
            if (check_adjacent_neighbor(neighbor[index_array[top]])) {
                solution.insert(index_array[top]);
            }
        }
    }

    if (check_PIDS(solution)) return solution;
    return {};
}

bool can_remove(const unordered_set<int>& node_neighbor) {
    for (int node : node_neighbor) {
        if (neighbor_popularity[node]-1.f < neighbor[node].size()/2.f) {
            return false;
        }
    }
    for (int node : node_neighbor)
        neighbor_popularity[node]--;

    return true;
}

unordered_set<int> remove_nodes(unordered_set<int> solution) {
    vector<int> index_array(neighbor.size());

    for (int i = 0; i < neighbor.size(); i++) index_array[i] = i;
    sort (index_array.begin(), index_array.end(), compare);
    reverse(index_array.begin(), index_array.end());
    
    for (int top = 0; top < neighbor.size(); top++) {
        if (solution.find(index_array[top]) != solution.end()) {
            if (can_remove(neighbor[index_array[top]])) {
                solution.erase(index_array[top]);
            }
        }
    }

    if (check_PIDS(solution)) return solution;
    return {};
}

void setNeighbor (vector <unordered_set<int>> s){
  neighbor = s;
}

vector<int> getNeighborPopularity() {
    return neighbor_popularity;
}

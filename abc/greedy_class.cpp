/***************************************************************************
    greedy.cpp
    (C) 2021 by C. Blum & M. Blesa

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
vector< unordered_set<int> > neighborsG;
vector<int> neighbors_popularity;


// C++ program to find the Dominant Set of a graph
using namespace std;

bool check_PIDS(unordered_set <int> subset) {
    for (auto s : neighborsG){
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

bool check_adjacent_neighbors(const unordered_set<int>& node_neighbors) {
    for (int node : node_neighbors) {
        if (neighbors_popularity[node] < neighborsG[node].size()/2.f) {
            for (int neighbor : node_neighbors)
                neighbors_popularity[neighbor]++;
            return true;
        }
    }
    return false;
}


//Cambiar subset a vector para guardar último nodo visitado y pasarlo por parámetro
int find_removable_node(unordered_set <int> subset) {
    unordered_set<int> aux = subset;
    for (int s : subset) {
        aux.erase(s);
        for (int n : neighborsG[s]) {
            neighbors_popularity[n]--;
        }

        if (!check_adjacent_neighbors(neighborsG[s])) return s;
        aux.insert(s);
    }
    return -1;
}
/*
// Linear time sort
vector<int> counting_sort(vector<int> array) {
    int max = neighbors.size()-1;
    vector<int> output (array.size());
    vector<int> count (max+1, 0);
    int size = max+1;
    // Store the count of each element
    for (int i = 0; i < size; i++) {
        count[array[i]]++;
    }

    // Store the cummulative count of each array
    for (int i = 1; i <= max; i++) {
        count[i] += count[i - 1];
    }

    // Find the index of each element of the original array in count array, and
    // place the elements in output array
    for (int i = size - 1; i >= 0; i--) {
        output[count[array[i]] - 1] = i;
        count[array[i]]--;
    }

    reverse(output.begin(), output.end());
    return output;
}

*/

bool compare(int i, int j) {
    return neighborsG[i].size() > neighborsG[j].size();
}

unordered_set<int> greedy() {
    unordered_set<int> solution;
    neighbors_popularity = vector<int>(neighborsG.size(), 0);
    vector<int> index_array(neighborsG.size());

    for (int i = 0; i < neighborsG.size(); i++) index_array[i] = i;
    sort (index_array.begin(), index_array.end(), compare);

    // if counting works:
    // index_array = counting_sort(index_array); //contains the nodes id from highest to lowest degree in O(n)

    for (auto n : neighborsG[3835]) cout << neighborsG[n].size() << endl;
    int pos = neighborsG.size()-1;
    while (pos >= 0 and neighborsG[index_array[pos]].size() == 0) --pos;
    while (pos >= 0 and neighborsG[index_array[pos]].size() == 1) {
        auto it = neighborsG[index_array[pos]].begin();
        solution.insert(*it);
        for (int neighbor : neighborsG[*it]) {
            neighbors_popularity[neighbor]++;
        }
        --pos;
    }

    for (int top = 0; top <= neighborsG.size()-1; top++) {
        if (solution.find(index_array[top]) == solution.end()) {
            if (check_adjacent_neighbors(neighborsG[index_array[top]])) {
                solution.insert(index_array[top]);
            }
        }
    }

    if (check_PIDS(solution)) return solution;
    return {};
}


bool can_remove(const unordered_set<int>& node_neighbors) {
    for (int node : node_neighbors) {
        if (neighbors_popularity[node]-1.f < neighborsG[node].size()/2.f) {
            return false;
        }
    }
    for (int node : node_neighbors)
                neighbors_popularity[node]--;

    return true;
}

unordered_set<int> remove_nodes(unordered_set<int> solution) {
    vector<int> index_array(neighborsG.size());

    for (int i = 0; i < neighborsG.size(); i++) index_array[i] = i;
    sort (index_array.begin(), index_array.end(), compare);

    for (int top = 0; top < neighborsG.size(); top++) {
        if (can_remove(neighborsG[index_array[top]])) {
            solution.erase(index_array[top]);
        }
    }

    if (check_PIDS(solution)) return solution;
    return {};
}

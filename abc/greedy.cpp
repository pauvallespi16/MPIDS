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

// global variables concerning the random number generator (in case needed)
time_t t;
Random* rnd;

// Data structures for the problem data
int n_of_nodes;
int n_of_arcs;
vector< unordered_set<int> > neighbors;
vector<int> neighbors_popularity;

// string for keeping the name of the input file
string inputFile;

// dummy parameters as examples for creating command line parameters
// see function read_parameters(...)
int dummy_integer_parameter = 0;
int dummy_double_parameter = 0.0;

// C++ program to find the Dominant Set of a graph
using namespace std;

inline int stoi(string &s) {

  return atoi(s.c_str());
}

inline double stof(string &s) {

  return atof(s.c_str());
}

void read_parameters(int argc, char **argv) {
    int iarg = 1;

    while (iarg < argc) {
        // if i write "-i" then the next string in the command line
        // has to represent the input file
        if (strcmp(argv[iarg],"-i")==0)
            inputFile = argv[++iarg];

        // example for creating a command line parameter param1
        //-> integer value is stored in dummy_integer_parameter
        else if (strcmp(argv[iarg],"-param1")==0)
            cout << dummy_integer_parameter << endl;

        // example for creating a command line parameter param2
        //-> double value is stored in dummy_double_parameter
        else if (strcmp(argv[iarg],"-param2")==0)
            dummy_double_parameter = atof(argv[++iarg]);

        iarg++;
    }
}

bool check_PIDS(unordered_set <int> subset) {
    for (auto s : neighbors){
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
        if (neighbors_popularity[node] < neighbors[node].size()/2.f) {
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
        for (int n : neighbors[s]) {
            neighbors_popularity[n]--;
        }

        if (!check_adjacent_neighbors(neighbors[s])) return s;
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
    return neighbors[i].size() > neighbors[j].size();
}

unordered_set<int> greedy() {
    unordered_set<int> solution;
    neighbors_popularity = vector<int>(neighbors.size(), 0);
    vector<int> index_array(neighbors.size());

    for (int i = 0; i < neighbors.size(); i++) index_array[i] = i;
    sort (index_array.begin(), index_array.end(), compare);

    // if counting works: 
    // index_array = counting_sort(index_array); //contains the nodes id from highest to lowest degree in O(n)

    int pos = neighbors.size()-1;
    while (pos >= 0 and neighbors[index_array[pos]].size() == 0) --pos;
    while (pos >= 0 and neighbors[index_array[pos]].size() == 1) {
        auto it = neighbors[index_array[pos]].begin();
        if (solution.find(*it) == solution.end()) {
            solution.insert(*it);
            for (int neighbor : neighbors[*it]) {
                neighbors_popularity[neighbor]++;
            }
            /*if (neighbors[*it].size() == 2) {
                for (int n : neighbors[*it]) {
                    if (n != index_array[pos]) {
                        solution.insert(n);
                        for (int nb : neighbors[n]) {
                            neighbors_popularity[nb]++;
                        }
                    }
                }
            }*/
        }
        --pos;
    }

    for (int top = 0; top < neighbors.size() ; top++) {
        if (solution.find(index_array[top]) == solution.end()) {
            if (check_adjacent_neighbors(neighbors[index_array[top]])) {
                solution.insert(index_array[top]);
            }
        }
    }

    if (check_PIDS(solution)) return solution;
    return {};
}


bool can_remove(const unordered_set<int>& node_neighbors) {
    for (int node : node_neighbors) {
        if (neighbors_popularity[node]-1.f < neighbors[node].size()/2.f) {
            return false;
        }
    }
    for (int node : node_neighbors)
                neighbors_popularity[node]--;

    return true;
}

unordered_set<int> remove_nodes(unordered_set<int> solution) {
    vector<int> index_array(neighbors.size());

    for (int i = 0; i < neighbors.size(); i++) index_array[i] = i;
    sort (index_array.begin(), index_array.end(), compare);
    reverse(index_array.begin(), index_array.end());
    
    for (int top = 0; top < neighbors.size(); top++) {
        if (solution.find(index_array[top]) != solution.end()) {
            if (can_remove(neighbors[index_array[top]])) {
                solution.erase(index_array[top]);
            }
        }
    }

    if (check_PIDS(solution)) return solution;
    return {};
}

/************
Main function
*************/
int main( int argc, char **argv ) {
    read_parameters(argc,argv);
    
    // setting the output format for doubles to 2 decimals after the comma
    std::cout << std::setprecision(10) << std::fixed;

    // initializing the random number generator.
    // A random number in (0,1) is obtained with: double rnum = rnd->next();
    rnd = new Random((unsigned) time(&t));
    rnd->next();

    // variables for storing the result and the computation time
    // obtained by the greedy heuristic
    double results = std::numeric_limits<int>::max();
    double time = 0.0;

    // opening the corresponding input file and reading the problem data
    ifstream indata;
    indata.open(inputFile.c_str());
    if(not indata) { // file couldn't be opened
        cout << "Error: file could not be opened" << endl;
    }

    indata >> n_of_nodes;
    indata >> n_of_arcs;
    neighbors = vector< unordered_set<int> >(n_of_nodes);
    int u, v;
    while(indata >> u >> v) {
        neighbors[u - 1].insert(v - 1);
        neighbors[v - 1].insert(u - 1);
    }
    indata.close();
    // the computation time starts now
    Timer timer;

    // Example for requesting the elapsed computation time at any moment:

    // HERE GOES YOUR GREEDY HEURISTIC
    // When finished with generating a solution, first take the computation
    // time as explained above. Say you store it in variable ct.
    // Then write the following to the screen:
    // cout << "value " << <value of your solution> << "\ttime " << ct << endl;

    /*    
    neighbors = vector<unordered_set<int> >(10);
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
    
    
    neighbors[0] = {1, 2};
    neighbors[1] = {0, 2};
    neighbors[2] = {0, 1, 3, 4};
    neighbors[3] = {2};
    neighbors[4] = {2, 7, 5};
    neighbors[5] = {4, 6};
    neighbors[6] = {5};
    neighbors[7] = {4};*/

    unordered_set<int> sol_set = greedy();
    if (check_PIDS(sol_set)) {
        if (check_PIDS(sol_set)) cout << "NODES:" << sol_set.size() << endl;
        /*int node = find_removable_node(sol_set);
        while (node != -1) {
            sol_set.erase(node);
            node = find_removable_node(sol_set);
        }*/
        sol_set = remove_nodes(sol_set);
        double ct = timer.elapsed_time(Timer::VIRTUAL);
        cout << "TIME:" << ct << endl;
        
        if (check_PIDS(sol_set)) cout << "NODES:" << sol_set.size() << endl;
        if (check_MPIDS(sol_set)) cout << "MINIMAL" << endl;
        else cout << "NOT MINIMAL" << endl;
    }
}
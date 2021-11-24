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
#include <limits>
#include <iomanip>

// global variables concerning the random number generator (in case needed)
time_t t;
Random* rnd;

// Data structures for the problem data
int n_of_nodes;
int n_of_arcs;
vector< set<int> > neighbors;

// string for keeping the name of the input file
string inputFile;

// dummy parameters as examples for creating command line parameters 
// see function read_parameters(...)
int dummy_integer_parameter = 0;
int dummy_double_parameter = 0.0;

// C++ program to find the Dominant Set of a graph
#include <bits/stdc++.h>
using namespace std;
  
inline int stoi(string &s) {

  return atoi(s.c_str());
}

inline double stof(string &s) {

  return atof(s.c_str());
}

bool check_PIDS(vector <int> subset) {
    set<int> sset (subset.size());
    for (int node : subset) sset.insert(node);
    for (set <int> s : neighbors){
        int count = 0;
        for (int i : s) {
            if (sset.find(i) != sset.end())
                count++; break;
            if (count < s.size()/2) return false;
        }
    }  
    return true;
}

/*
if there is one iteration (i) with false, we know it's not dominant so break
*/
bool check_MPIDS(vector <int> subset) {
   for (int s : subset) {
       bool found = false;
       for (int i=0; i<neighbors.size(); i++){
           bool foundIn = false;

           // find out if the node is connected to any vertex from the subset
           for (int j : subset) {
               if (j != s && i == j) {found = true; break;}
               if (j != s && neighbors[i].find(j) != neighbors[i].end()) {found = true; break;} 
           }
           
           // if the vertex is not found, then we have found no cases in which the node isn't connected
           if (!foundIn) {found = true; break;}
       }

        // signifies that no node is missing a connection when removing the node, graph is still dominant
        if (!found) return false;
   }
   return true;
}

bool comparePairs (pair <int, int> p1, pair <int, int> p2){
    if (p1.second == p2.second)
        return p1.first < p2.first;
    return p1.second > p2.second;
}

void pan_greedy () {
    int n = neighbors.size();

    vector <pair <int, int> > aux (n);
    for (int i=0; i<n; i++){
        aux[i] = pair <int, int> (i, neighbors[i].size());
    }
    sort(aux.begin(), aux.end(), comparePairs);

    for (int i=0; i<n; i++) {

    }
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

/************
Main function
*************/

int main( int argc, char **argv ) {

    /*read_parameters(argc,argv);
    
    // setting the output format for doubles to 2 decimals after the comma
    std::cout << std::setprecision(2) << std::fixed;

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
    neighbors = vector< set<int> >(n_of_nodes);
    int u, v;
    while(indata >> u >> v) {
        neighbors[u - 1].insert(v - 1);
        neighbors[v - 1].insert(u - 1);
    }
    indata.close();
    
    for (auto val : neighbors) {
        cout << "Neighbour: " << endl;
        for (auto s : val) {
            cout << s << endl;
        }
        cout << endl;
    }

    // the computation time starts now
    Timer timer;*/

    // Example for requesting the elapsed computation time at any moment: 
    // double ct = timer.elapsed_time(Timer::VIRTUAL);

    // HERE GOES YOUR GREEDY HEURISTIC
    // When finished with generating a solution, first take the computation 
    // time as explained above. Say you store it in variable ct.
    // Then write the following to the screen: 
    // cout << "value " << <value of your solution> << "\ttime " << ct << endl;


    vector <pair <int, int>> aux = {pair<int, int>(0, 10), pair<int, int>(1, 20), pair<int, int>(2, 20), pair<int, int>(3, 2)};
    sort(aux.begin(), aux.end(), comparePairs);

    for (pair <int, int> a : aux){
        cout << a.first << " " << a.second << endl;
    }
}
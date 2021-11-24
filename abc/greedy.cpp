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
#include "Basics.h"
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
#include <unordered_set>
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

    neighbors = vector<set<int> >(8);
    /*neighbors[0] = {5, 7};
    neighbors[1] = {2};
    neighbors[2] = {1, 6, 8, 9};
    neighbors[3] = {6};
    neighbors[4] = {6};
    neighbors[5] = {0, 7, 9};
    neighbors[6] = {2, 3, 4};
    neighbors[7] = {0, 5};
    neighbors[8] = {2};
    neighbors[9] = {2, 5};*/

    neighbors[0] = {1, 2};
    neighbors[1] = {0, 2};
    neighbors[2] = {0, 1, 3, 4};
    neighbors[3] = {2};
    neighbors[4] = {2, 7, 5};
    neighbors[5] = {4, 6};
    neighbors[6] = {5};
    neighbors[7] = {4};


    Basics b;

    vector<int> subcjt = {2, 3, 4, 5};
    unordered_set<int> uset;
    for (int i : subcjt) uset.insert(i);
    cout << (b.check_MPIDS(uset) ? "yes" : "no") << endl;
}

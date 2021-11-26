/***************************************************************************
    local_search.cpp
    (C) 2021 by C.Blum & M.Blesa

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
#include "greedy_class.cpp"
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
vector< unordered_set<int> > neighbors;

// string for keeping the name of the input file
string inputFile;

// number of applications of local search
int n_apps = 1;

// dummy parameters as examples for creating command line parameters ->
// see function read_parameters(...)
int dummy_integer_parameter = 0;
int dummy_double_parameter = 0.0;

//heuristics
bool modified;
int node1, node2; //node1 is added/removed, node2 is for switch added
string op;
int incoming_colored_nodes;

//simulated annealing
double T = 1;
const double Tmin = 0.0001;
const double alpha = 0.95;
const double numIterations = 100;

inline int stoi(string &s) {

  return atoi(s.c_str());
}

inline double stof(string &s) {

  return atof(s.c_str());
}

unordered_set<int> fromArray (vector <int> x){
  unordered_set <int> s;
  for (int i : x) s.insert(i);
  return s;
}

bool addNodeToSolution (unordered_set<int>& s, int n) {
  if (s.find(n) == s.end()) { 
    node1 = n;
    modified = true;
    op = "add";  
    s.insert(n); return true; 
  }
  modified = false;
  return false;
}

bool removeFromSolution (unordered_set<int>& s, int n) {
  if (s.find(n) != s.end()) { 
    node1 = n;
    modified = true;
    op = "remove";
    s.erase(n); return true; 
  }
  modified = false;
  return false;
}

bool switchNodes (unordered_set<int>& s, int n1, int n2){
  if (s.find(n1) != s.end() && s.find(n2) == s.end()) { 
    s.erase(n1); s.insert(n2);
    node1 = n1; node2 = n2;
    modified = true;
    op = "switch";
    return true;
  }
  modified = false;
  return false;
}

int calcHeuristics (unordered_set<int> sAux){
  if (!modified) return incoming_colored_nodes;

  if (op == "add")
    incoming_colored_nodes += neighbors[node1].size();

  else if (op == "remove"){
    incoming_colored_nodes -= neighbors[node1].size();

    for (int x : neighbors[node1]){
      int count = 0;
      for (int i : neighbors[x]){
        if (sAux.find(i) != sAux.end())
          count++;
      }
      if (count < sAux.size()/2.f) incoming_colored_nodes += neighbor.size();
    }
  }

  else if (op == "switch"){
    incoming_colored_nodes -= neighbors[node1].size();

    for (int x : neighbors[node1]){
      int count = 0;
      for (int i : neighbors[x]){
        if (sAux.find(i) != sAux.end())
          count++;
      }
      if (count < sAux.size()/2.f) incoming_colored_nodes += neighbor.size();
    }

    incoming_colored_nodes += neighbors[node2].size();
  }

  return incoming_colored_nodes;
}

///////////////////////////////////////////////////////////////////////////////
//                          SIMULATED ANNEALING                              //
///////////////////////////////////////////////////////////////////////////////

unordered_set<int> nextNeighborSimulated (unordered_set<int> s){
  int x = rnd -> next()*3; // 0..1
  if (x == 0){
    int n = rnd -> next()*neighbors.size();
    addNodeToSolution(s, n);
  }
  else if (x == 1) {
    int n = rnd -> next()*neighbors.size();
    removeFromSolution(s, n);
  } else {
    int n1 = rnd -> next()*neighbors.size();
    int n2 = rnd -> next()*neighbors.size();
    switchNodes(s, n1, n2);
  }
  return s;
}


unordered_set <int> simulatedAnnealing (unordered_set <int> s){
  unordered_set <int> minS;
  double min = INT_MAX;

  unordered_set <int> currSol = s;
  double curr = calcHeuristics(currSol);

  while (T > Tmin) {
    for (int i=0; i<numIterations; i++){
      if (curr < min){
        min = curr;
        minS = currSol;
        cout << "here" << endl;
      }

      unordered_set <int> neigh = nextNeighborSimulated(currSol);
      double newNeigh = calcHeuristics(neigh);

      double ap = pow(M_E, curr - newNeigh/T);
      if (ap > rnd -> next()){
        currSol = neigh;
        curr = newNeigh;
      }
    }

    T *= alpha;
  }
  return minS;
}

///////////////////////////////////////////////////////////////////////////////
//                               HILL CLIMBING                               //
///////////////////////////////////////////////////////////////////////////////

void findNeighborsHillClimbing (unordered_set <int> s, 
  vector <unordered_set<int>>& n, vector <int>& heurs){
  unordered_set <int> x;
  for (int i = 0; i < neighbors.size(); i++){
    x = s;
    if (addNodeToSolution (x, i)){
      heurs.push_back(calcHeuristics(x));
      n.push_back(x);
    }

    for (int j : s){
      x = s;
      if (switchNodes(x, i, j)){
        heurs.push_back(calcHeuristics(x));
        n.push_back(x);
      }
    }
  }

  for (int i : s){
    x = s;
    if (removeFromSolution(s, i)){
      heurs.push_back(calcHeuristics(x));
      n.push_back(x);
    }
  }
}

void hillClimbing (unordered_set <int>& s){
  bool foundMin = true;
  while (foundMin){
    foundMin = false;

    int curHeur = calcHeuristics (s);
    vector <unordered_set<int>> n;
    vector <int> hNext;

    findNeighborsHillClimbing(s, n, hNext);
    for (int i=0; i < n.size(); i++){
      int next = hNext[i];
      //cout << next << endl;
      if (curHeur > next){
        cout << next << endl;
        foundMin = true;
        s = n[i]; curHeur = next; 
      }
    }
  }
}

void read_parameters(int argc, char **argv) {

    int iarg = 1;

    while (iarg < argc) {
        if (strcmp(argv[iarg],"-i")==0) inputFile = argv[++iarg];

        // reading the number of applications of local search
        // from the command line (if provided)
        else if (strcmp(argv[iarg],"-n_apps")==0) n_apps = atoi(argv[++iarg]);

        // example for creating a command line parameter param1 ->
        // integer value is stored in dummy_integer_parameter
        else if (strcmp(argv[iarg],"-param1")==0) {
            dummy_integer_parameter = atoi(argv[++iarg]);
        }
        // example for creating a command line parameter param2 ->
        // double value is stored in dummy_double_parameter
        else if (strcmp(argv[iarg],"-param2")==0) {
            dummy_double_parameter = atof(argv[++iarg]);
        }
        iarg++;
    }
}


/**********
Main function
**********/

int main( int argc, char **argv ) {

    read_parameters(argc,argv);

    // setting the output format for doubles to 2 decimals after the comma
    std::cout << std::setprecision(2) << std::fixed;

    // initializing the random number generator.
    // A random number in (0,1) is obtained with: double rnum = rnd->next();
    rnd = new Random((unsigned) time(&t));
    rnd->next();

    // vectors for storing the result and the computation time
    // obtained by the <n_apps> applications of local search
    vector<double> results(n_apps, std::numeric_limits<int>::max());
    vector<double> times(n_apps, 0.0);

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

    /*neighbors = vector<unordered_set<int> >(10);
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

    n_of_arcs = 0;
    for (unordered_set<int> s : neighbors) {
      n_of_arcs += s.size();
    }

    setNeighbor (neighbors);
    unordered_set <int> sAux = greedy();
    for (int s : sAux) {
      cout << s << " ";
    }
    cout << "done greedy" << endl;

    hillClimbing(sAux);

    for (int s : sAux) {
      cout << s << " ";
    }
    cout << endl;*/

    // main loop over all applications of local search
    for (int na = 0; na < n_apps; ++na) {

        // the computation time starts now
        Timer timer;

        // Example for requesting the elapsed computation time at any moment:
        // double ct = timer.elapsed_time(Timer::VIRTUAL);

        cout << "start application " << na + 1 << endl;

        n_of_arcs = 0;
        for (unordered_set<int> s : neighbors) {
          n_of_arcs += s.size();
        }

        setNeighbor (neighbors);
        unordered_set <int> sAux = greedy();

        for (int s : sAux){
          cout << s << " ";
        }
        cout << endl;

        modified = false;
        node1 = node2 = 0;
        op = "";

        incoming_colored_nodes = 0;
        for (unordered_set <int> s : neighbors){
            int count = 0;
            for (int i : s) {
                if (sAux.find(i) != sAux.end())
                    count++;
            }
            if (count < s.size()/2.f) incoming_colored_nodes += neighbor.size();
            else incoming_colored_nodes += count;
        }

        cout << "INITIAL: " << incoming_colored_nodes << endl;
        cout << sAux.size() << endl;

        // HERE GOES YOUR LOCAL SEARCH METHOD

        //g.neighbors = greedy();

        cout << rnd -> next() << endl;

        sAux = simulatedAnnealing(sAux);

        for (int s : sAux){
          cout << s << " ";
        }
        cout << endl;

        cout << sAux.size() << endl;

        /*for (int s : sAux) {
          cout << s << " ";
        }*/
        cout << endl;

        // The starting solution for local search may be randomly generated,
        // or you may incorporate your greedy heuristic in order to produce
        // the starting solution.

        // Whenever you move to a new solution, first take the computation
        // time as explained above. Say you store it in variable ct.
        // Then, write the following to the screen:
        // cout << "value " << <value of the current solution>;
        // cout << "\ttime " << ct << endl;

        // When a local minimum is reached, store the value of the
        // corresponding solution in vector results:
        // results[na] = <value of the local minimum>;

        // Finally store the needed computation time (that is, the time
        // measured once the local minimum is reached) in vector times:
        // times[na] = ct;

        cout << "end application " << na + 1 << endl;
        /*for (int s : sAux){
          cout << s << " ";
        }
        cout << endl;*/
    }

    // calculating the average of the results and computation times, and
    // their standard deviations, and write them to the screen
    double r_mean = 0.0;
    int r_best = std::numeric_limits<int>::max();
    double t_mean = 0.0;
    for (int i = 0; i < int(results.size()); i++) {
        r_mean = r_mean + results[i];
        if (int(results[i]) < r_best) r_best = int(results[i]);
        t_mean = t_mean + times[i];
    }
    r_mean = r_mean/double(results.size());
    t_mean = t_mean/double(times.size());
    double rsd = 0.0;
    double tsd = 0.0;
    for (int i = 0; i < int(results.size()); i++) {
        rsd = rsd + pow(results[i]-r_mean,2.0);
        tsd = tsd + pow(times[i]-t_mean,2.0);
    }
    rsd = rsd/double(results.size());
    if (rsd > 0.0) {
        rsd = sqrt(rsd);
    }
    tsd = tsd/double(results.size());
    if (tsd > 0.0) {
        tsd = sqrt(tsd);
    }
    cout << r_best << "\t" << r_mean << "\t" << rsd << "\t";
    cout << t_mean << "\t" << tsd << endl;
}

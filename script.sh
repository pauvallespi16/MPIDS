#!/bin/bash
echo
echo ------- ALGORITHMICS -------
echo Posibilities of names of files:
echo - greedy
echo - local_search
echo - metaheuristic
echo - cplex
echo ----------------------------
echo
echo Introduce the name of the file
read namefile
echo

if [[ "$namefile" == "greedy" || "$namefile" == "local_search" ]]; then
    path="Part_1"
else
    path="Part_2"
fi
echo Introduce number of executions
read napps

cd $path
if [[ "$namefile" != "cplex" ]]; then 
    echo
    make
    echo
    echo graph_football
    ./$namefile -i ../input/instances/graph_football.txt -n_apps $napps
    echo
    echo graph_jazz
    ./$namefile -i ../input/instances/graph_jazz.txt -n_apps $napps
    echo
    echo ego-facebook
    ./$namefile -i ../input/instances/ego-facebook.txt -n_apps $napps
    echo
    echo graph_actors_dat
    ./$namefile -i ../input/instances/graph_actors_dat.txt -n_apps $napps
    echo
    echo graph_CA-AstroPh
    ./$namefile -i ../input/instances/graph_CA-AstroPh.txt -n_apps $napps
    echo
    echo graph_CA-CondMat
    ./$namefile -i ../input/instances/graph_CA-CondMat.txt -n_apps $napps
    echo
    echo graph_CA-HepPh
    ./$namefile -i ../input/instances/graph_CA-HepPh.txt -n_apps $napps
    echo
    echo socfb-Brandeis99
    ./$namefile -i ../input/instances/socfb-Brandeis99.txt -n_apps $napps
    echo
    echo socfb-Mich67
    ./$namefile -i ../input/instances/socfb-Mich67.txt -n_apps $napps
    echo
    echo soc-gplus
    ./$namefile -i ../input/instances/soc-gplus.txt -n_apps $napps
    echo
else
    cd ILP_CPLEX/executable/Debug/
    ./$namefile
fi

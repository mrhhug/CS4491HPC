#!/bin/bash

#echo "gcc version 4.9.1 20140903"
#echo "GNU bash, version 4.3.30"
#echo "flags are as follows:"
#echo "gcc -Wall -O3 ./profs_merge.c -pthread -o profs_merge.o"
#echo "gcc -Wall -O3 ./my_merge.c -pthread -o my_merge.o"
#echo ""
#echo "ARRAYSIZE is size ofset to be sorted"
#echo "THRES is when the merge is large enought to spawn threads"

export SUPPRESSSUCCES=1
echo "THRES, ARRAYSIZE,myMergeTime,profMergeTime"
TT="$(date +%s)"
for j in 1000 10000 20000 50000
do
	for i in 500000 700000 800000 1000000 5000000
	do
		export THRES=$j
		export ARRAYSIZE=$i
		T="$(date +%s)"
		./my_merge.o
		myMergeTime="$(($(date +%s)-T))"
		T="$(date +%s)"
		./profs_merge.o
		profMergeTime="$(($(date +%s)-T))"
		echo $j,$i,$myMergeTime,$profMergeTime
	done
done
duration="$(($(date +%s)-TT))"
echo "The tests from for "$duration" seconds"

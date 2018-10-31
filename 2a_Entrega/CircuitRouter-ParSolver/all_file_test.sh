#!/bin/bash

errcnt=0
success=0
total=$2
numthreads=$1
for file in ../inputs/*.txt; do
    if [ $(basename $file) != "random-x512-y512-z7-n512.txt" ] ; then
        for i in $(seq 1 $total); do
            ./doTest.sh $numthreads inputs/$(basename $file) &> /dev/null
            errcnt=$((errcnt + $(grep -c ",," ../results/$(basename $file).speedups.csv) ))
        done
    fi
done

total=$((total * 11 * $1))
success=$((total - errcnt))
percentage=$(echo "scale=2; ${success}*100/${total}" | bc)
echo errors: $errcnt
echo sucesses: $success
echo percentage: ${percentage}%
beep

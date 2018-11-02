#!/bin/bash

make > /dev/null
make -C ../CircuitRouter-SeqSolver > /dev/null

errcnt=0
total=$(($2 * $1))
file=inputs/random-x32-y32-z3-n64.txt
for i in $(seq 1 $2); do
    ./doTest.sh $1 $file &> /dev/null
    errcnt=$((errcnt + $(grep -c ",," ../results/$(basename $file).speedups.csv) ))
done

success=$((total - errcnt))
percentage=$(echo "scale=2; ${success}*100/${total}" | bc)

echo errors: $errcnt
echo sucesses: $success
echo percentage: ${percentage}%
beep

make clean > /dev/null
make clean -C ../CircuitRouter-SeqSolver > /dev/null
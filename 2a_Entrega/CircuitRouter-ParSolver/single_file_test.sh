#!/bin/bash

make > /dev/null
make -C ../CircuitRouter-SeqSolver > /dev/null

errcnt=0
total=$(($2 * $1))
file=random-x32-y32-z3-n64.txt
paths=0
max=0
nthreads=0
temp=0
for i in $(seq 1 $2); do
    ./doTest.sh $1 inputs/$file &> /dev/null
    errcnt=$((errcnt + $(grep -c ",," ../results/$file.speedups.csv) ))
    paths=$((paths + $(cut -c 19-21 <(grep "Paths routed" ../inputs/$file.res))))
    for f in $(seq 1 $1); do
        temp=$(cut -c 13- <(grep "$f," ../results/$file.speedups.csv))
        if (( $(echo "$temp > $max" |bc -l) )); then
            max=$temp
            nthreads=$f
        fi
    done
done

success=$((total - errcnt))
percentage=$(echo "scale=2; ${success}*100/${total}" | bc)

echo "========================================="
echo "| errors: $errcnt                             |"
echo "| sucesses: $success                          |"
echo "| percentage: ${percentage}%                   |"
echo "| average paths: $(echo "scale = 2; $paths / $2 " | bc)                  | "
echo "| max speedup: $max for $nthreads threads   |"
echo "========================================="
beep

make clean > /dev/null
make clean -C ../CircuitRouter-SeqSolver > /dev/null
#!/bin/bash

make > /dev/null
make -C ../CircuitRouter-SeqSolver > /dev/null

errcnt=0
avgspd=0
total=$(($2 * $1))
file=inputs/random-x128-y128-z3-n128.txt
paths=0
max=0
nthreads=0
temp=0
for i in $(seq 1 $2); do
    ../doTest.sh $1 $file &> /dev/null
    errcnt=$((errcnt + $(grep -c ",," ../$file.speedups.csv) ))
    paths=$((paths + $(cut -c 19-21 <(grep "Paths routed" ../$file.res))))
    for f in $(seq 1 $1); do
        temp=$(cut -c 13- <(grep "$f," ../$file.speedups.csv))
        avgspd=$(echo "$avgspd + $temp" | bc)
        if (( $(echo "$temp > $max" |bc -l) )); then
            max=$temp
            nthreads=$f
        fi
    done
done

success=$((total - errcnt))

echo "========================================="
echo " errors: $errcnt"
echo " sucesses: $success"
echo " percentage: $(echo "scale=2; ${success}*100/${total}" | bc)%"
echo " average paths: $(echo "scale = 2; $paths / $2 " | bc)"
echo " max speedup: $max for $nthreads threads"
echo " average speedup: $(echo "scale = 6; $avgspd / $total " | bc)"
echo "========================================="
#beep

make clean > /dev/null
make clean -C ../CircuitRouter-SeqSolver > /dev/null

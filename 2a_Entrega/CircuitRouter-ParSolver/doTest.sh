#!/bin/bash

TIMEFORMAT=%R #Time now returns only the real time

cd ../solver
make > /dev/null
cd ../paralel

#Filename handling and generating output file
file=$(basename $2)
output="../results/"$file".speedups.csv"
input="../"$2
exectime=$input".res"
touch $output
echo "#threads,exec_time,speedup" > $output

#Sequencial Solver time and print to file for reference
../solver/CircuitRouter-SeqSolver "$input">/dev/null
seqtime=$(cut -c 19-27 <(grep "Elapsed" $exectime))

echo "1S,$seqtime,1" >> $output

#Testing all thread numbers
for i in $(seq 1 $1); do
    partime=$(time (./teste.sh>/dev/null 2>&1) 2>&1)
    speedup=$(echo "scale=6; ${seqtime}/${partime}" | bc)
    echo "$i,$partime,$speedup" >> $output
done


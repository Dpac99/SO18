#!/bin/bash

#Filename handling and generating output filename
filename=$(basename $2)
output="../results/"$filename".speedups.csv"
input="../"$2
exectime=$input".res"
touch $output

echo "#threads,exec_time,speedup" > $output

#Sequencial Solver time and print to file for reference
../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver "$input" >/dev/null
seqtime=$(grep "Elapsed" $exectime | cut -c 19-27)

echo "1S,$seqtime,1" >> $output

#Testing with all threads from 1 to $1
for i in $(seq 1 $1); do
    ./CircuitRouter-ParSolver -t $i $input >/dev/null 
    
    partime=$(grep "Elapsed" $exectime | cut -c 19-27)
    speedup=$(echo "scale=6; ${seqtime}/${partime}" | bc)

    echo "$i,$partime,$speedup" >> $output
done
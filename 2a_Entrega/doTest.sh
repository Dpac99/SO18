#!/bin/bash
if (( $# == 2)); then
    #Filename handling and generating output filename
    input=$2
    output=$input".speedups.csv"
    results=$input".res"
    touch $output

    echo "#threads,exec_time,speedup" > $output

    #Sequencial Solver time and print to file for reference
    ./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver "$input" >/dev/null
    seqtime=$(grep "Elapsed" $results | cut -c 19-27)

    echo "1S,$seqtime,1" >> $output

    #Testing with all threads from 1 to $1
    for i in $(seq 1 $1); do
        ./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $input >/dev/null 
        
        partime=$(grep "Elapsed" $results | cut -c 19-27)
        speedup=$(echo "scale=6; ${seqtime}/${partime}" | bc)

        echo "$i,$partime,$speedup" >> $output
    done
else 
    echo Invalid number of arguments
    echo Usage:
    echo ./doTesh.sh [NUMTHREADS] [PATH/TO/INPUTFILE]
fi
#!/bin/bash

for f in $(ls ~/Desktop/SO/inputs/*.txt)
do
    echo
    echo filename: $f
    cat $f | wc -l
    grep -c p $f
done


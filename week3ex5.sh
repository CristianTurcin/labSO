#!/bin/bash

director=$1
out_file=$2
nr=0

if [ $# -ne 2 ]
then
    echo "not enough params"
fi

for fisier in "$director"/*txt
do
    count=$(wc -m < $fisier)
    total=$(($total + $count))
   
done
echo "nume: $fisier nr caractere: $count" >> $out_file
echo "total: $total" >> $out_file

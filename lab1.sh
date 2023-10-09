#!#!/bin/bash

if test "$#" -gt 3
then
    echo "da"
else
    echo "nu"

fi

#2  for (( i=2 ; i<=$# ; i++ ))
cnt=0
file1=$1
dir=$2
shift 2

for arg
do
   
    if test "$arg" -gt 10
    then
	cnt=`expr $cnt + 1`
    fi
done

echo "$cnt"

#3
sum=0

for arg
do
    sum=`expr $sum + $arg`
done

echo "$sum"

#4

echo "$sum" | wc -L

#5

if test -f $file1
then
    echo "e obisnuit"
    echo "count=$cnt; sum=$sum" > $file1
else
    echo "nu e "    
fi

#6












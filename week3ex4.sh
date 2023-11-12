#!/bin/bash

director=$1
caracter=$2


if [ $# -ne 2 ]
then
    echo "not enough params"
fi

for fisier in "$director"/*.txt
do
    if [ -f $fisier ]
    then
	chmod +$caracter $fisier
    else
	echo "nu exista fisier" 
    fi
    
done




























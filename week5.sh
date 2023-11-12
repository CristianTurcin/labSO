#!/bin/bash

dir=$1
file=$2

count=0
countRec=0
if [ $# -ne 2 ]
then
    echo "not enough params"
fi


for entry in "$dir"/*.txt
do
    if [ -L $entry ] 
    then
	count=$((count + 1))
        
    elif [ -f $entry ]
    then
	rez= cat $entry | grep -E '^[A-Z][a-zA-Z0-9\,\ ]*\.$' | grep -E -v 'si[\ ]*\,' | grep -E -v 'n[pb]+' 
	if [ ! -z $rez ]
	then
	    echo $entry >> $file
	fi

       
	
    elif [ -d $entry ]
    then
	countRec=`bash $0 $entry $file`
	count=$((count + countRec))
	
    fi
    echo $count
    
       
done

echo $count

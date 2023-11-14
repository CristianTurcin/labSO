#!/bin/bash

# IP address ipv4 format range from 0.0.0.0 to 255.255.255.255

# salveaza in file2 doar ip-urile cu formatul ipv4


file1=$1
file2=$2

while read line
   do
	echo $line | grep -E '^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$' >> $file2
  done < $file1


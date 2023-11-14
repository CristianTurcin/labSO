#!/bin/bash

# REGULI PROPOZITIE
# -poate contine litere mari, mici sau cifre
# -poate contine spatii
# -poate contine virgula
# -incepe cu litera mare 
# -se termina cu punct
# -dupa "si" sa nu fie "," si/sau " ,"
# -"n" ce apare inaintea unui "p" sau "b"



while read line
    do
	echo $line | grep -E '^[A-Z][a-zA-Z0-9\,\ ]*\.$' | grep -E -v 'si[\ ]*\,' | grep -E -v 'n[pb]+'
    done



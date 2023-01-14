#!/bin/bash

#Josephine Coates
#CS288, Spring 2022
#Prof. Weiwei Jia


#Write a shell script which:
#1. accepts a filename
#2. checks if file exists
#3. if file exists, copy the file to the same name + .bak + the current time

if [ -f $1 ] ;
then
	cp $1 $1".bak"$(date +"%T")
else
	echo "It's not a file, or the file does not exist."
	exit 1
fi

#!/bin/bash

#Josephine Coates
#CS288, Spring 2022
#Prof. Weiwei Jia


#Write a Bash script that removes all zero length ordinary files in the directory
#(including those in the sub-directories at all levels) passed as an optional argument.
#If no directory is specified, assume current directory
#(Do not use the find command)

if [ $# -gt 0 ];
then
	cd $1
fi

nextdirectory(){
	for file in $(ls)
	do
		if [ -d $file ];
		then
			cd $file
			nextdirectory
		else
			if [ ! -s $file ]; #if empty
			then
				echo "$file is empty"
				rm $file
			else
				echo "$file is not empty"
			fi
		fi
	done
	cd ..
}

nextdirectory

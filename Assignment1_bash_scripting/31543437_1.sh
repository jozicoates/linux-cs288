#!/bin/bash

#Josephine Coates
#CS288, Spring 2022
#Prof. Weiwei Jia


#Write a Bash script that takes the name of a file or directory as an argument,
#1. reports if it is a directory or a file, and if the user has read, write and execute permission on the file or directory, and
#2. if it is a file, reports the size of the file and the category of the file based on its size.

check_permissions(){
	if [ -r $1 ];
	then
		echo "The user has read permission on the $var1."
	fi
	if [ -w $1 ];
	then
		echo "The user has write permission on the $var1."
	fi
	if [ -x $1 ];
	then
		echo "The user has execute permission on the $var1."
	fi
}


if [ -d $1 ];
then
	echo "This is a directory."
	var1="directory"
	check_permissions
fi
	

if [ -f $1 ];
then
	echo "This is a file."
	var1="file"
	check_permissions
	filesize=$(du -b $1 | cut -f1) #get filesize
	if [ $filesize -gt 1048576 ];
	then
		echo "The size of the file is: $filesize"
		echo "This is a large file."
	elif [ $filesize -gt 102400 ];
	then
		echo "The size of the file is: $filesize"
		echo "This a medium file."
	else
		echo "The size of the file is: $filesize"
		echo "This is a small file."
	fi
fi

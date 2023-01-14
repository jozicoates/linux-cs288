#!/bin/bash

#Josephine Coates
#CS288, Spring 2022
#Prof. Weiwei Jia


#Write a Bash script that takes a list of numbers as arguments and outputs the sum

let sum=0

for n in $@
do
	let "sum = $sum + $n"
done
echo $sum

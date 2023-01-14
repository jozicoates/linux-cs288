#!/bin/bash

#Josephine Coates
#CS288, Spring 2022
#Prof. Weiwei Jia


#Write a bash script to find out the 15 most frequently used words on a set of Wikipedia pages (AA, AB, AC, ... ZZ)

for i in {A..Z};
do
	for j in {A..Z};
	do
		wget https://en.wikipedia.org/wiki/$i$j -O $i$j.html
		lynx -dump -nolist $i$j.html > $i$j.txt
		grep -o "[[:alpha:]]\+" $i$j.txt >> count.txt
		rm $i$j.html
		rm $i$j.txt

	done
done

grep -o "[[:alpha:]]\+" count.txt | sort -f | uniq -i -d -c | sort -f -n -r >> sort.txt
#piping: get matching words from count.txt, sort them (ignore case), only count duplicate lines (ignore case), sort by descending count (ignore case), output to sort.txt
head -15 sort.txt
#print top 15 lines of sort.txt



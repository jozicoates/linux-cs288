#Josephine Coates
#CS288, Spring 2022
#Prof. Weiwei Jia

#Write a Python program to find out the 15 most frequently used words on a set of Wikipedia pages (AA, AB, AC, ... ZZ)

import urllib.request
from bs4 import BeautifulSoup
import re

matchDict = dict()

for i in 'abcdefghijklmnopqrstuvwxyz':
	for j in 'abcdefghijklmnopqrstuvwxyz':
		url = "https://en.wikipedia.org/wiki/" + i + j
		file = urllib.request.urlopen(url)
		str_html = file.read()
		obj_html = BeautifulSoup(str_html, "html.parser")

		for element in obj_html(["script", "style"]):
			element.extract()

		str_text = obj_html.get_text()
		str_text.lower()
		
		regex = r'\b[a-z]+\b'
		wordlist = re.findall(regex, str_text)
		
		for match in wordlist:
			if match in matchDict:
				matchDict[match] += 1
			else:
				matchDict[match] = 1

#sort wordlist array
counter = 1
dct = {v:k for k,v in sorted(matchDict.items(), key=lambda item:item[1], reverse=True)}

for k,v in dct.items():
	if counter > 15:
		break
	print(k,v)
	counter += 1


//Josephine Coates
//CS288, Spring 2022
//Prof. Weiwei Jia

//Write a C program that sorts the environment variables passed to the program based on environmment variable names.
//Sort the environment variables in ascending order by applying strcmp() on their names


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void insertionSort(char* elems[], int size){
	char* delim = "=";
	int prev;
	
	for (int i = 0; i < size; i++){
		prev = i - 1;
		char* j = elems[i];
		
		while(prev >= 0){
			char* curr = calloc(strlen(elems[prev+1])+ 1, 1);
			char* cpy = calloc(strlen(elems[prev]) + 1, 1);
			
			strcpy(curr, elems[prev+1]);
			strcpy(cpy, elems[prev]);
			
			int comp = strcmp(strtok(curr, delim), strtok(cpy, delim));
			
			if (comp >= 0){
				break;
			}
			
			//switch positions
			elems[prev+1] = elems[prev];
			elems[prev] = j;
			j = elems[prev];
			prev--;
			
			free(curr);
			free(cpy);
		}
	}

}

int main(int argc, char* argv[], char* envp[]){
	
	int index;

	while(envp[index]){
		index++;
	}
	insertionSort(envp, index);
	
	for(int i = 0; i < index; i++){
		printf("%s\n", envp[i]);
	}
	printf("\n");
	return 0;	
}


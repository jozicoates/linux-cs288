//Josephine Coates
//CS 288, Spring 2022
//Prof. Weiwei Jia


//Write a C program that inverts the bits of a non-negative number provided by user through argument
//Invert the order of bits, not flip the bits
//Print out the number, its binary representation, followed by the inverted number and its binary representation

#include <stdio.h>
#include <stdlib.h>

void dectobin(unsigned int decn, char* binarr);
void reverseBits(char* binarr, char* revbinarr);
void bintodec(char* binarr, unsigned int decn);
void printarr(char* binarr);

int main(int argc, char ** argv){
    unsigned int num = atoi(argv[1]);
    unsigned int revnum;
    char* bin = (char*)malloc(32);
    char* revbin = (char*)malloc(32);

    //PRINT DECIMAL NUM
    printf("%u:\t\t", num);

    //CONVERT DECIMAL TO BINARY
    dectobin(num, bin);
    printarr(bin);
    printf("\n");

    //REVERSE BITS OF BINARY (don't print yet)
    reverseBits(bin, revbin);

    //PRINT REVERSE DECIMAL NUM
    bintodec(revbin,revnum);

    //PRINT REVERSE BITS OF BINARY
    printarr(revbin);
    printf("\n");


    return 0;
}

void dectobin(unsigned int decn, char* binarr){
    for (int i = 31; i >= 0; i--){
        binarr[i] = decn & 0x1;
        decn >>= 1;
    }
}

void reverseBits(char* binarr, char* revbinarr){
    for (int i = 31; i >= 0; i--){
        revbinarr[i] =  binarr[31-i];
    }
}

void bintodec(char* binarr, unsigned int decn){
    int e = 0;
    for (int i = 31; i >= 0; i--){
        if (binarr[i] == 1){
            decn += (1 << e);
        }
        e++;
    }
    printf("%u:\t\t", decn);
}


void printarr(char* binarr){
    for (int i = 0; i < 32; i++){
        if (i % 4 == 0 && i != 0){
            printf(" %d", binarr[i]);
        }
        else{
            printf("%d", binarr[i]);
        }
    }
}

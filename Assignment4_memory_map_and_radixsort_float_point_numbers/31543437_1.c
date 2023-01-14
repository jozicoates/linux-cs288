//Josephine Coates
//Prof. Jia Weiwei
//Spring 2022

//Implement a C program that sorts a set of 4-byte float point values in ascending order using radix sort. The values are saved in a file.
//The program should read/write the file through memory mapping. When the program finishes, the sorted values should be saved in the same file.

//Must work directly on binary data. Program only needs two buckets to help with sorting.
//Some numbers are negative (handle the problem caused by sign bits).


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

const int floatSize = (4 << 3);

void reverseArray(float* array, int size){
    /* Swaps elements at start of array with elements at end of the array*/
    for(int i = 0; i < (size / 2); i++){
        float temp = array[size - 1 - i];
        array[size - 1 - i] = array[i];
        array[i] = temp;
    }
}

void radixSort(float* arr, unsigned int length){
    double sum = 0;

    //creating buckets
    float* bucket1 = (float*)malloc(length * floatSize);
    float* bucket2 = (float*)malloc(length * floatSize);

    unsigned int index1 = 0, index2 = 0;


    for (int i = 0; i < floatSize; i++){
        for (int j = 0; j < length; j++){
            if (i == 0){
                sum += arr[j];
            }
            unsigned int convert = * (unsigned int*) &arr[j]; //converts to bit
            int currBit = (convert >> i) & 1;

            if (currBit == 0){
                bucket1[index1] = arr[j];
                index1++;
            }
            else{
                bucket2[index2] = arr[j];
                index2++;
            }
        }

        if (i == (floatSize - 1)){
            reverseArray(bucket2, index2);
            memcpy(arr, bucket2, (index2 + 1) * floatSize);
            memcpy(arr + index2, bucket1, (index1 + 1) * floatSize);
        }
        else{
            memcpy(arr, bucket1, (index1 + 1) * floatSize);
            memcpy(arr + index1, bucket2, (index2 + 1) * floatSize);
        }

        memset(&bucket1[0], 0, floatSize * length);
        memset(&bucket2[0], 0, floatSize * length);

        index1 = 0;
        index2 = 0;
    }

    printf("Total is: %f\n", sum);

    free(bucket1);
    free(bucket2);

}

int main(int argc, char* argv[]){

    if (argc < 2){
        printf("Invalid argument.\n");
        exit(1);
    }

    char* path = argv[1];
    printf("File name is: %s\n", path);

    int fd; //file descriptor
    float* map;
    int size, count;
    struct stat st;

    //open file and get file size
    fd = open(path, O_RDWR | O_CREAT); //CHANGED
    fstat(fd, &st);
    size = st.st_size;
    count = size / floatSize;

    printf("File size is: %d\n", size);

    //if file can't be opened
    if (fd == -1){
        perror("Error- file cannot be opened.\n");
        exit(1);
    }

    //map file to virtual memory
    map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    

    radixSort(map, count);

    close(fd);
    return 0;
}

//Josephine Coates
//CS288, Spring 2022
//Prof. Weiwei Jia


//Write a C program that creates multiple processes to multiply two matrices of float point values. One is saved in a file, the other is an identity matrix.
//The product should be the same as the first matrix (use cmp to compare)
//# of proc, file containing first matrix, and file saving product matrix specified in CL

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

/* MULTIPLY 2 MATRICIES OF FLOATS (multiple processes)
 * given matrix * identity matrix
 * save in another file
 */

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Wrong number of arguments.\n");
        exit(1);
    }

    int numProc = atoi(argv[1]);
    char* matrixfile = argv[2];
    char* savefile = argv[3];

    //OPEN INPUT FILE
    int fd;
    int fileSize, count;
    struct stat statBuf;
    float* map;

    fd = open(matrixfile, O_RDWR);
    if (fd == -1){
        printf("Error opening file\n");
        exit(1);
    }

    fstat(fd, &statBuf);
    fileSize = statBuf.st_size;

    //map file to virtual memory
    map = mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //special cases to exit
    if (numProc > (fileSize / sizeof(float))){
        printf("Process greater than matrix size\n");
        exit(1);
    }
    if(numProc == 0){
        printf("Process count is 0\n");
        exit(1);
    }
    if (map == NULL){
        exit(1);
    }

    //CREATE MATRIX
    int n = (int)sqrt(fileSize / sizeof(float)); //size of row/col
    float matrix[n][n];
    float idmatrix[n][n];

    int index;
    
    //matrix from mmapped file
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            matrix[j][i] = map[index];
            index++;
        }
    }

    //identity matrix
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (i == j){
                idmatrix[i][j] = 1;
            }
            else{
                idmatrix[i][j] = 0;
            }
        }
    }


    //OPEN OUTPUT FILE
    int result;
    float* writeMap;

    fd = open(savefile, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600); //read + write, no exec
    if (fd == -1){
        printf("Error opening file to write to\n");
        exit(1);
    }

    result = lseek(fd, fileSize - 1, SEEK_SET);
    if (result == -1){
        close(fd);
        printf("Error calling lseek()\n");
        exit(1);
    }

    result = write(fd, "", 1);
    if (result == -1){
        close(fd);
        printf("Error writing last byte of file\n");
        exit(1);
    }

    writeMap = mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (writeMap == MAP_FAILED){
        close(fd);
        printf("Error mmapping the file\n");
        exit(1);
    }

    //CREATE CHILD PROCESSES
    pid_t pid, child[numProc];
    int status, valuesRead = 0;

    int work = (fileSize / sizeof(float)) / numProc;
    int rmwork = (fileSize / sizeof(float)) % numProc + work;
    int size = sqrt(fileSize / sizeof(float));

    for (int i = 0; i < numProc; i++){ //i = creation index
        if ((child[i] = fork()) < 0){
            printf("Error forking child process\n");
            exit(1);
        }
        else if(child[i] == 0){ //child processes working
            int workamt;
            if (i != numProc - 1){
                workamt = work;
            }
            else{
                workamt = rmwork;
            }
            int workingRows[workamt];
            int workingCols[workamt];
            int matrixrow = 0, matrixcol = 0;

            int changeamt = work * i;

            for (int j = 0; j < changeamt; j++){
                if (++matrixrow >= size){ //overflow onto next col
                    matrixrow = 0;
                    matrixcol++;
                }
            }

            for (int j = 0; j < workamt; j++){
                workingRows[j] = matrixrow;
                workingCols[j] = matrixcol;
                matrixrow++;
                if (matrixrow >= size){ //overflow onto next col
                    matrixrow = 0;
                    matrixcol++;
                }
            }

            float matrixValue = 0;
            int workValue = 0;

            for (int j = 0; j < size; j++){
                for (int m = 0; m < size; m++){
                    for (int n = 0; n < size; n++){
                        matrixValue += matrix[m][n] * idmatrix[n][j];
                    }
                    if ((m == workingRows[workValue]) && (j == workingCols[workValue])){
                        printf("(Process[%d]: PID[%d] PPID[%d]) - writing at [%d][%d] value: %f\n", i, getpid(), getppid(), m, j, matrixValue);
                        writeMap[changeamt++] = matrixValue;
                        workValue++;
                    }
                    matrixValue = 0;
                }
            }
            exit(0);
        }
    }

    //WAIT- parents wait for every child to die before continue
    while (numProc > 0){
        pid = wait(&status);
        numProc--;
    }

    if (pid != 0){
        if (munmap(writeMap, fileSize) == -1){
            printf("Error unmapping the file\n");
            exit(1);
        }
        close(fd);
    }

}
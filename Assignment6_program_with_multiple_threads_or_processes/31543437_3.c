//Josephine Coates
//CS288, Spring 2022
//Prof. Weiwei Jia


//Write a C program that creates multiple processes to calculate an approximation of pi.
//The number of processes and number of terms required should be specified in CL.
//You may choose an IPC method (pipe, FIFO, shn) you like (but pipe fits most)


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

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Wrong number of arguments.\n");
        exit(1);
    }

    int numProc = atoi(argv[1]);
    int numPlaces = atoi(argv[2]);
    long double sum[numProc];
    pid_t pid, child[numProc];
    int status;
    
    //IPC variables
    int pfds[2];
    char buf[30];

    pipe(pfds); //open IPC pipe as parent

    //CREATE CHILD PROCESSES
    for (int i = 0; i < numProc; i++){
        if ((child[i] = fork()) < 0){
            printf("Error forking child proccess\n");
            exit(1);
        }
        else if(child[i] == 0){
            //calculate value of pi
            int myrank, j, k, m;
            double h, x, partialsum;
            h = 1.0 / (double) numPlaces;
            j = (numPlaces / numProc) * i + 1;
            k = (numPlaces / numProc) * (i + 1);
            m = j;

            for (j; j <= k; j++){
                x = h * ((double)j - 0.5);
                partialsum += 4.0 / (1.0 + x * x);
            }

            sprintf(buf, "%f", partialsum);
            buf[strlen(buf)] = 0;
            write(pfds[1], buf, strlen(buf) + 1);
            printf("(Process [%d] PID[%d] PPID[%d]) %5d%5d\t%f\n", i, getpid(), getppid(), m, k, partialsum);

            exit(0);
        }
        else{
            read(pfds[0], buf, sizeof(buf));
            sum[i] = atof(buf);
        }
    }

    double numProc2 = numProc;

    while (numProc2 > 0){
        pid = wait(&status);
        numProc2--;
    }

    //calculate approx for pi
    double mypi = 0.;
    for (int i = 0; i < numProc; i++){
        mypi += (1.0 / (double) numPlaces) * sum[i];
    }
    printf("%f\n", mypi);

}
//Josephine Coates
//CS288, Spring 2022
//Prof. Weiwei Jia


//Write a C program that creates multiple worker threads to calculate the sum of float point values saved in a file.
//Each thread calculates a partial sum, and the main thread adds up the partial sums.

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

/* CREATES MULTIPLE WORKER THREADS
 * calculates sum of float pt values and save in a file
 * each thread calculates partial sum, 
 * main calculates total sum
 * CL- program, # workers, file containing values
 */

struct Worker{
    int id;
    int start;
    int size;
};

float* map; //global variable for mapping file to virtual mem
float sum; //gloabl variable for final sum

void* start(void* worker){
    struct Worker currworker = *(struct Worker*) worker;
    float partialsum = 0;
    printf("Thread[%d] starting work:\n", currworker.id);

    for (int i = currworker.start; i < currworker.start + currworker.size; i++){
        float value = map[i];
        partialsum += value;
    }

    printf("Thread[%d] finished working, added partial sum: %f\n", currworker.id, partialsum);
    sum += partialsum;

    pthread_exit(NULL);
}


int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong number of arguments.\n");
        exit(1);
    }

    int workerct = atoi(argv[1]);
    char* file = argv[2];

    //open file
    int fd;
    int fileSize, count;
    struct stat statBuf;

    fd = open(file, O_RDWR | O_CREAT);
    if (fd == -1){
        printf("Error opening file\n");
        exit(1);
    }

    fstat(fd, &statBuf);
    fileSize = statBuf.st_size;
    
    //map file to virtual memory
    map = mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //if file cannot be read
    if (map == NULL){
        exit(1);
    }

    //WORKER CREATION
    struct Worker* workers = (struct Worker*)malloc(sizeof(struct Worker) * workerct);
    int work = (fileSize / sizeof(float)) / workerct; //work per thread
    int rmwork = (fileSize / sizeof(float) % workerct) + work; //remainder work for last thread

    for (int i = 0; i < workerct; i++){
        workers[i].id = i;
        if (i != workerct - 1){
            workers[i].size = work;
        }
        else{
            workers[i].size = rmwork;
        }
        workers[i].start = i * work;
    }

    printf("Threads: %d\nFile: %s\n\n", workerct, file);
    

    //THREAD CREATION
    pthread_t threads[workerct];
    int status;

    for (int i = 0; i < workerct; i++){
        status = pthread_create(threads+i, NULL, start, &workers[i]);
        if (status != 0){
            perror("Pthread_create");
            exit(1);
        }
    }

    //wait for all threads before terminating
    for (int i = 0; i < workerct; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Sum: %f\n", sum);

}

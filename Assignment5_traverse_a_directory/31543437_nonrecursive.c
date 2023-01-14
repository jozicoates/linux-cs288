//Josephine Coates
//Prof. Jia Weiwei
//Spring 2022

/* TRAVERSING A DIRECTORY ITERATIVELY
 *
 * recursively going thru the directories (nextDirectory)
 * adding file sizes and names to an array (nextDirectory)
 * which is then sorted (sort)
 * and printed (main)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdbool.h>
#include <fcntl.h>
#define MAX_FILES 10000




struct File{
    char* fpath;
    int fsize;
};

struct File* fptr; //points to base address of file array
int findex; //what index of array 


void nextDirectory(char* input){

    struct stat statBuf; //create stat object
    mode_t mode;
    int result = lstat(input, &statBuf); //returns info about filePath itself, not link it refers to
    
    //obtain file status
    if (result == -1){
        printf("Cannot read '%s' \n", input);
        return;
    }

    mode = statBuf.st_mode; //file type

    if (S_ISDIR(mode)){ //if directory
        DIR* dir;
        struct dirent* dirEntry;
        char* fileName;
        char* filePath = input;

        dir = opendir(filePath);

        //read all elem in dir
        while((dirEntry = readdir(dir)) != NULL){
            fileName = dirEntry->d_name;

            //check for eof
            if (dirEntry == NULL){
                break;
            }
            //ignore parent and current dir
            if (strcmp(fileName, ".") != 0 && strcmp(fileName, "..") != 0){
                fileName = malloc(strlen(input) + strlen(dirEntry->d_name) + 2);
                sprintf(filePath, "%s/%s", input, fileName); //append fileName onto path

                result = lstat(filePath, &statBuf);
                mode = statBuf.st_mode;

                if (result == -1){
                    printf("Cannot read '%s' \n", filePath);
                    return;
                }

                if(S_ISDIR(mode)){ //directory
                    dir = opendir(filePath);
                    continue;
                }
                else if (S_ISREG(mode)){ //regular file
                    (fptr + findex)->fpath = filePath;
                    (fptr + findex)->fsize = statBuf.st_size;
                    findex++;
                }
            }
        }
        closedir(dir);
        
    }
    else if(S_ISREG(mode)){ //if regular file
        (fptr + findex)->fpath = input;
        (fptr + findex)->fsize = statBuf.st_size;
        findex++;
    }

}

void sort(struct File* files){
    for (int i = 0; i < findex; i++){
        for (int j = i+1; j < findex; j++){
            if (files[i].fsize > files[j].fsize){
                struct File temp = files[i];
                files[i] = files[j];
                files[j] = temp;
            }
        }
    }
}




int main(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    struct File files[MAX_FILES];
    fptr = files;
    findex = 0;

    nextDirectory(argv[1]);
    sort(fptr);

    for(int i = 0; i < findex; i++){
        printf("%d\t%s\n", fptr->fsize, fptr->fpath);
        fptr++;
    }

    return 0;
}
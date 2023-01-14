//Josephine Coates
//Prof. Jia Weiwei
//Spring 2022

/* TRAVERSING A DIRECTORY RECURSIVELY
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



void nextDirectory(char* filePath){

    struct stat statBuf; //create stat object
    mode_t mode;
    int result = lstat(filePath, &statBuf); //returns info about filePath itself, not link it refers to
    
    //obtain file status
    if (result == -1){
        printf("Cannot read '%s' \n", filePath);
        return;
    }

    mode = statBuf.st_mode; //file type

    if (S_ISDIR(mode)){ //if directory
        DIR* dir;
        struct dirent* dirEntry;
        char* fileName;

        dir = opendir(filePath);

        //read all elem in dir
        for(;;){
            dirEntry = readdir(dir);

            //check for eof
            if (dirEntry == NULL){
                break;
            }
            //ignore parent and current dir
            if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0){
                fileName = malloc(strlen(filePath) + strlen(dirEntry->d_name) + 2);
                sprintf(fileName, "%s/%s", filePath, dirEntry->d_name);
                nextDirectory(fileName);
            }
        }
        closedir(dir);
        
    }
    else if(S_ISREG(mode)){ //if regular file
        (fptr + findex)->fpath = filePath;
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
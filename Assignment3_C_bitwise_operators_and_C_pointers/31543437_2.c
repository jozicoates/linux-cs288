//Josephine Coates
//CS288, Spring 2022
//Prof. Weiwei Jia


//Write a C program that uses getopt() to parse its command line
//Extract options, option parameters, and other arguments


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//OPTIONS
int main(int argc, char * argv[]){
    int options;

    while ((options = getopt(argc, argv, ":cduf:s:")) != -1){ //-1 is end of option list

        switch(options){
            case 'c':
            case 'd':
            case 'u': //count, repeat, or unique
                printf("Option: %c\n", options);
                break;
            case 'f':
                printf("Fields: %s\n", optarg);
                break;
            case 's':
                printf("Chars: %s\n", optarg);
                break;
            case ':':
                printf("Option %c needs arguments\n", optopt);
                break;
            case '?':
                printf("Unknown option character: %c\n", optopt);
                break;
        }
    }



    //FILES
    int fileCt = 0;
    while(optind < argc){ //stop when index of next arg > # args
        if (strstr(argv[optind], ".txt")){
            if (fileCt == 0){
                printf("Input File: %s\n", argv[optind]);
            }
            else{
                printf("Output File: %s\n", argv[optind]);
            }
            fileCt++;
        }
        printf("Extra argument: %s\n", argv[optind]);
        optind++;
    }

    return 0;
}

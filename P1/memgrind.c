#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"


void immediate_1(){
    for(int i = 0; i < 120; i++){
        char *immediate = malloc(1);
        free(immediate);
    }
}

void arrayDeallocate_2(){
    double *ptrs[120];
    for(int i = 0; i < 120; i++){
        ptrs[i] = malloc(1);
    }
    
    for(int i = 0; i < 120; i++){
        free(ptrs[i]);
    }
}

void allocDeallocRandom_3(){
    //rand()
    srand(time(0));

    //variables
    double *ptrs[120];
    int currSize = 0;
    int totalAlloc = 0;

    while(totalAlloc < 120){
        int random = rand() % 2;

        if(random == 0){ //allocate
            totalAlloc += 1;
            ptrs[currSize] = malloc(1);
            currSize += 1;
        }else{ //deallocate
            if(currSize != 0){
                free(ptrs[currSize - 1]);
                currSize -= 1;
            }else{
                free(ptrs[currSize]); //Let the error happen
            }
        }
    }

    for(int i = 0; i < currSize; i++){
        free(ptrs[i]);
    }
}

void seriesOfAllocDealloc_4(){
    double *ptrs[256];
    for(int i = 0; i < 120; i++){
        ptrs[i] = malloc(1);
    }
    
    for(int i = 60; i < 120; i++){
        free(ptrs[i]);
    }

    for(int i = 60; i < 256; i++){
        ptrs[i] = malloc(1);
    }

    for(int i = 0; i < 256; i++){
        free(ptrs[i]);
    }
}

void freeEvenFreeOdd_5(){
    double *ptrs[256];
    for(int i = 0; i < 256; i++){
        ptrs[i] = malloc(1);
    }

    for(int i = 0; i < 256; i += 2){
        free(ptrs[i]);
    }

    for(int i = 1; i < 256; i += 2){
        free(ptrs[i]);
    }
}

int main(int argc, char *argv[]){

    //Invalid argument
    if(argv[1] == NULL){
        printf("INVALID INPUT\n");
        return 1;
    }else if((*argv[1] - '0') > 5){
        printf("INVALID INPUT\n");
        return 1;
    }

    //Make before and after time
    struct timeval before, after;
    double time_elapsed;

    switch(*argv[1] - '0'){
        case 1:
            for(int i = 0; i < 50; i++){
                gettimeofday(&before, NULL);
                immediate_1();
                gettimeofday(&after, NULL);
                time_elapsed += (double) (after.tv_usec - before.tv_usec)/1000000;
            }
            break;
        case 2:
            for(int i = 0; i < 50; i++){
                gettimeofday(&before, NULL);
                arrayDeallocate_2();
                gettimeofday(&after, NULL);
                time_elapsed += (double) (after.tv_usec - before.tv_usec)/1000000;
            }
            break;
        case 3:
            for(int i = 0; i < 50; i++){
                gettimeofday(&before, NULL);
                allocDeallocRandom_3();
                gettimeofday(&after, NULL);
                time_elapsed += (double) (after.tv_usec - before.tv_usec)/1000000;
            }
            break;
        case 4:
            for(int i = 0; i < 50; i++){
                gettimeofday(&before, NULL);
                seriesOfAllocDealloc_4();
                gettimeofday(&after, NULL);
                time_elapsed += (double) (after.tv_usec - before.tv_usec)/1000000;
            }
            break;
        case 5:
            for(int i = 0; i < 50; i++){
                gettimeofday(&before, NULL);
                freeEvenFreeOdd_5();
                gettimeofday(&after, NULL);
                time_elapsed += (double) (after.tv_usec - before.tv_usec)/1000000;
            }
            break;
            
    }

    //print output difference
    printf("Time elapsed: %f seconds\n", time_elapsed/50);

    return 0;
}
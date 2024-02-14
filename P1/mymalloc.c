#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <string.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];
#define heap ((char *) memory)

void *mymalloc(size_t n, char *file, int line) {
    chunk chunky = {7, 444};
    printf("%ld\n", sizeof(chunk));
    for(int i = 0; i < sizeof(chunk); i++){
        heap[i] = ((char *) &chunky)[i];
    }

    chunk chunkyboi;

    for(int i = 0; i < sizeof(chunk); i++){
        ((char *) &chunkyboi)[i] = heap[i];
    }
    printf("data: %d, %d\n", chunkyboi.size, chunkyboi.alloc_stat);
    
}

int main(){
    malloc(5);
    return 0;
}

void myfree(void *ptr, char *file, int line) {
    
}

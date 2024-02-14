#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


#define MEMLENGTH 512
static double memory[MEMLENGTH];


void *mymalloc(size_t n, char *file, int line) {
    void *ptr;
    chunk new_chunk = {n, 1, ptr};

    char *heapstart = (char *) memory;
    

}

void myfree(void *ptr, char *file, int line) {
    
}
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <string.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];
#define heap ((char *) memory)

void printtheheader(){
    printf("1. %d\n", heap[0]);
    printf("2. %d\n", heap[1]);
    printf("3. %d\n", heap[2]);
    printf("4. %d\n", heap[3]);
    printf("5. %d\n", heap[4]);
    printf("6. %d\n", heap[5]);
    printf("7. %d\n", heap[6]);
    printf("8. %d\n", heap[7]);
}

void printheader(header h){
    for(int i = 0; i < sizeof(header); i++){
        ((char *) &h)[i] = heap[i];
    }
    printf("data: %d, %d\n", h.size, h.alloc_stat);
}

// malloc should be the only thing "updating" chunk size
// pointer returned by malloc must to the first byte of the payload
void *mymalloc(size_t n, char *file, int line) { 

    //checks if the first header exists
    if(heap[0] == 0 && heap[1] == 0){
        header head = {4088, 0};
        for(int i = 0; i < sizeof(header); i++){// assigning header object to the first 8 bytes of the heap
            heap[i] = ((char *) &head)[i];
        }
    } 

    int pl_size = (pl_size % 8 != 0) ? (n + 8 - (n%8)) : (n);

    

    //if (h)

    header deletemelater;
    header *ptr = &deletemelater;
    return ptr;
    
}

int main(){
    malloc(5);
    return 0;
}

void myfree(void *ptr, char *file, int line) { // free takes 
    
}

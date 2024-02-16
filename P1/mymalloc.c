#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <string.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];
#define heap ((char *) memory)
#define headersize sizeof(header)

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
    for(int i = 0; i < headersize; i++){
        ((char *) &h)[i] = heap[i];
    }
    printf("data: %d, %d\n", h.size, h.alloc_stat);
}

header readheader(int i){
    header ret;
    for(int j = i; j < i + headersize; j++){
        ((char *) &ret)[j - i] = heap[j];
    }
    return ret;
}

void writeheader(int i, header insert){
    for(int j = i; j < i + headersize; j++){
        heap[j] = ((char *) &insert)[j - i];
    }
}

// malloc should be the only thing "updating" chunk size
// pointer returned by malloc must to the first byte of the payload

void *mymalloc(size_t n, char *file, int line) { 

    //checks if the first header exists
    if(heap[0] == 0 && heap[1] == 0){
        header head = {4088, 0};
        for(int i = 0; i < headersize; i++){// assigning header object to the first 8 bytes of the heap
            heap[i] = ((char *) &head)[i];
        }
    }

    int pl_size = (n % 8 != 0) ? (n + 8 - (n%8)) : (n); // rounds up n, the size of the payload

    int i = 0;
    while(i < 4095){
        header read = readheader(i); //read current header
        printf("heap[%d]: %p\n", i, &heap[i]);

        //delete later, if the read header is not a header
        if(heap[0] == 0 && heap[1] == 0){
            printf("not a header, ummmmohhhhh\n");
        }
        
        if(read.alloc_stat == 0 && read.size >= pl_size){ //if current header is unallocated and has enough space

            if(read.size - pl_size >= 16){ // if there is enough space for another chunk to exist after this chunk (16 bytes)
                header rest = {read.size - pl_size - headersize, 0};
                writeheader(i + headersize + pl_size, rest); //create a new header for the free space after current chunk

                read.size = pl_size;
            }
            
            read.alloc_stat = 1; //allocate current chunk
            writeheader(i, read);

            return &heap[i + headersize]; //return payload pointer

        }
        
        i += headersize + read.size; //move to the next header
        
    }

    //no space to allocate - send error
    printf("dang it, no space\n"); //change later

    return NULL;
    
}

void myfree(void *ptr, char *file, int line) { // free takes 
    
}

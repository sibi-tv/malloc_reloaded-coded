#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <string.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];
#define heap ((char *) memory)
#define headersize sizeof(header)

header readheader(int i){
    header ret;
    for(int j = i; j < i + headersize; j++){
        ((char *) &ret)[j - i] = heap[j];
    }
    return ret;
}

int get_payload_size(void *ptr) {
    int index = 0;
    void *hptr = ptr-8;
    for (int i = 0; i < 4096; i++) {
        if ((char*)hptr == &heap[i]) {
            index = i;
            i = 5000;
        }
    }
    header homie = readheader(index);
    return homie.size;
}

void writeheader(int i, header insert){
    for(int j = i; j < i + headersize; j++){
        heap[j] = ((char *) &insert)[j - i];
    }
}

int isheader(int i) {
    if (i >= 0 && i < 4079) {
        header heady = readheader(i);
        if ((heady.size < 8 || heady.size > 4088)) {
            // if you don't have a header
            return 0;
        }
        return 1;
    }
    return 0;
}

void freepayload (int index) { // index of header, that has the payload info
    header h = readheader(index);
    for (int i = index + headersize; i < index + h.size + headersize; i++) {
        heap[i] = 0;
    }
}

// malloc should be the only thing "updating" chunk size
// pointer returned by malloc must to the first byte of the payload

void *mymalloc(size_t n, char *file, int line) { 

    if(n < 1){ //if header not found in memory
        fprintf(stderr, "\n* MEMORY ERROR * --> invalid input: value cannot be less than 1 (%s:%d)\n\n", file, line);
        return NULL;
    }

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
    fprintf(stderr, "\n* MEMORY ERROR * --> malloc: not enough storage to satisfy requested byte count (%s:%d)\n\n", file, line); //change later

    return NULL;
    
}

void myfree(void *ptr, char *file, int line) { // free takes 
    
    header free_header;
    int free_header_index = 0;

    int i = 0;
    for (; i < 4096; i++) {
        if ((char*)ptr == &heap[i]) {
            free_header_index = i-8;
            free_header = readheader(free_header_index);
            i = 5000;
        }
    }

    if(i != 5001){ //if header not found in memory
        fprintf(stderr, "\n* MEMORY ERROR * --> invalid free: header not found in memory (%s:%d)\n\n", file, line);
        return;
    }else if(free_header.alloc_stat == 0){
        fprintf(stderr, "\n* MEMORY ERROR * --> free: payload has already been deallocated/has never been allocated before/invalid payload pointer (%s:%d)\n\n", file, line);
        return;
    }

    //Check the left
    int prev_header_index = free_header_index-16;

    if (prev_header_index >= 0) {
        while (prev_header_index >= 0) { //find the previous header
            
            if (isheader(prev_header_index)) {
                break;
            }
            prev_header_index -= 8;
        } 

        header prev_header = readheader(prev_header_index);
        if (prev_header.alloc_stat == 0) { // if "left header is free"
            prev_header.size += free_header.size + headersize;
            writeheader(prev_header_index, prev_header);
            freepayload(free_header_index); // frees the payload

            // this for-loop frees the header itself
            for (int i = free_header_index; i < free_header_index + headersize; i++) {
                heap[i] = 0;
            }

            free_header = prev_header;
            free_header_index = prev_header_index;
        }
    } else {
        freepayload(free_header_index);
    }

    int next_header_index = free_header_index + free_header.size + headersize;
    if (next_header_index < 4096) {
        header next_header = readheader(next_header_index);
        if (next_header.alloc_stat == 0) {
            free_header.size += next_header.size + headersize;
            writeheader(free_header_index, free_header);
            freepayload(next_header_index);

            for (int i = next_header_index; i < next_header_index + headersize; i++) {
                heap[i] = 0;
            }
        }
    } else { // you are at the last chunk in memory
        freepayload(free_header_index);
    }
    
    free_header.alloc_stat = 0;
    writeheader(free_header_index, free_header);

    return;
}
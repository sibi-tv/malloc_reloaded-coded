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

int isheader(int i) {
    if (i >= 0 && i < 4079) {
        header heady = readheader(i);
        //printheader(heady);
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
        
        //DELETE LATER, each header jump
        //printf("heap[%d]: %p\n", i, &heap[i]);

        //delete later, if the read header is not a header
        if(heap[0] == 0 && heap[1] == 0){
            printf("not a header, ummmmohhhhh\n");
        }
        
        if(read.alloc_stat == 0 && read.size >= pl_size){ //if current header is unallocated and has enough space

            if(read.size - pl_size >= 16){ // if there is enough space for another chunk to exist after this chunk (16 bytes)
                header rest = {read.size - pl_size - headersize, 0};
                writeheader(i + headersize + pl_size, rest); //create a new header for the free space after current chunk
                
                //DELETE LATER, returns last header size
                //printf("second: %d\n", readheader(i + headersize + pl_size).size);

                read.size = pl_size;
            }
            
            read.alloc_stat = 1; //allocate current chunk
            writeheader(i, read);

            //DELETE LATER
            printf("Chunk created at address: %p\n", &heap[i + headersize]);

            return &heap[i + headersize]; //return payload pointer

        }
        
        i += headersize + read.size; //move to the next header
        
    }

    //no space to allocate - send error
    printf("ERROR --> malloc: not enough storage to satisfy requested byte count (%s:%d)\n", file, line); //change later

    return NULL;
    
}

void myfree(void *ptr, char *file, int line) { // free takes 
    
    header free_header;
    int free_header_index = 0;
    for (int i = 0; i < 4096; i++) { // change this to 4088?
        if ((char*)ptr == &heap[i]) {
            //printf("booyah\n");
            free_header_index = i-8;
            free_header = readheader(free_header_index);
        }
    }

    if (free_header == NULL) {//doesn't work
        printf(":D\n");
    }


    // free errors

    // Show that we retrieve the correct header from the ting
    //printf("%d\n", free_header_index);
    //printheader(free_header);
    //header rest_space = readheader(free_header_index+headersize+free_header.size);
    //printheader(rest_space);

    //Check the left
    int prev_header_index = free_header_index-16;

    if (prev_header_index >= 0) {
        while (prev_header_index >= 0) { //find the previous header
            //printf("%d\n", isheader(prev_header_index)); 
            
            if (isheader(prev_header_index)) {
                break;
            }
            prev_header_index -= 8;
        } 

        printf("%d\n", prev_header_index);
        header prev_header = readheader(prev_header_index);
        printf("%d\n", prev_header.alloc_stat);
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

    printf("%d\n", free_header_index);
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

    printheader(free_header);

    return;
}

/**
 * 
 * Runtime errors:
 * 1. (Malloc error) Out of memory --> 114 (DONE)
 * 2. (Free error) Freeing a bad pointer --> 135
 * 
 * 
 * Coming up with testing strategy
 * 1. What requirements do we have?
 * - Library must detect and report runtime errors
 * - mymalloc should allocate memory on our heap array
 * - myfree should free memory on our heap array
 * - mymalloc should align all allocated memory (5 --> 8), (11 --> 16)
 * - myfree should coalesce when necessary
 * 
 * 2. How do we know if these requirements were violated
 * - Proper error messages aren't printed
 * - If we allocate 4088 bytes and then try to malloc() again and are able to allocate more memory
 * - If we are not able to malloc again on a chunk we just freed
 * - Malloc does not allot memory divisible by 8
 * - When checking the header, we do not get the coalecsed size
 * 
 * 3. Write specific test cases
 * - Combine bullet points 1 & 2 into 1 test case: Test to see if a) malloc works b) if we get the proper runtime error (return null) --> malloc(4088) malloc(100) --> 2nd malloc should return NULL with error message
 * - Malloc 2 chunks, free 1 of the chunks, malloc into that chunk again --> returning success
 * - White-Box Test: Call malloc(n) where n is some number not divisible by 8. Add a method in mymalloc.c to spit out chunk size and use it in memtest to see if it equals n + 8 - (n%8).
 * - White-Box Test: In memtest, make 2 adjacent pointers & store the value p1.size + p2.size + 8 in a variable. Call free(*p1), then Call free(*p2). Add method to mymalloc.c to return 1 if 4088 bytes are free and 0 otherwise
 * --> Use test cases we already made in test.c
 * 
 * Memgrind:
 * 
 * 1. malloc() and immediately free() a 1-byte object, 120 times.
 * 2. Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
 * 3. Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), 
 *    until you have allocated 120 times. Deallocate any remaining objects.
 * - Keep a count of how many tings have been allocated by malloc
 * - If count == 0 and you get a free --> let the error happen
 * - If count == n, where n != 0 and you get a free --> free(arr[n-1])
 * - Free any remaining ptrs
 * 4. Allot 120 chunks to memory, free 120 at random, then free the rest.
 * 5. Allot 120 chunks to memory, free every other ting, then randomly free the rest
 * 
 * Makefile: Kareem
 * 
 * If *ptr does not exist within memory 
 * 
 * 
 * Additional readme info:
 * - Names + NetIDs
 * - Implementation (Design notes)
 * - Make file description?
 * - Contents of memtest
 * - Contents of memgrind
 * - poem professing love to rahulraj
 * 
*/

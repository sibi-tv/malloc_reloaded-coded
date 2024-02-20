#include <stdio.h>

#include "mymalloc.h"

void println() {
    printf("------------------------------------------------------------------------\n");
}

int main(){

    char *p = malloc(4088);
    free(p);
    free(p);
    return 1;

    // Test 0: Test if any freeing works
    int *ptr1 = malloc(199); //208
    int *ptr2 = malloc(16); //24
    
    free(ptr2);
    println();

    // Test 1: Free a memory allocation at the front of memory
    int *ptr3 = malloc(55); //64
    free(ptr1);
    println();

    // Test 2: Free memory with a free block on the left
    int *ptr4 = malloc(30); // 40
    int *ptr5 = malloc(10); // 24
    free(ptr4);
    free(ptr5);
    println();

    // Test 3: Free memory with a free block on the right
    int *ptr6 = malloc(30); // 40
    int *ptr7 = malloc(10); // 24
    free(ptr7);
    free(ptr6);
    println();

    // Test 4: Free memory between 2 free blocks
    int *ptr8 = malloc(96);
    int *ptr9 = malloc(96);
    free(ptr8); //data: 96, 0
    free(ptr3); //data: 3880, 0
    free(ptr9); //data: 4088, 0
    println();
    

    // Test 5: Free memory allocated at the end of memory;
    int *ptr10 = malloc(4072);
    int *ptr11 = malloc(8);
    free(ptr11);
    
    

}
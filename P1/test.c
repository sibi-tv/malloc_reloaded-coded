#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

void println() {
    printf("------------------------------------------------------------------------\n");
}

void case1() { // Does malloc Allocate? Does malloc display run-time error correctly?
    char *ptr1 = malloc(4088); //mallocing the entire heap
    char *ptr2 = malloc(1); //trying to malloc just 1 additional byte
    if (ptr2 == NULL) {
        printf("Test case 1 returns CORRECTLY\n");
    } else {
        printf("Test case 1 returns INCORRECTLY\n");
    }
    free(ptr1);
}

void case2() { // Does free Deallocate? Does freeing from the beginning of memory cause any bounds errors? Does free display run-time error correctly?
    char *ptr1 = malloc(199);   
    free(ptr1);
    free(ptr1);
    printf("Test 2 is CORRECT if there is a *MEMORY ERROR* message above. It is incorrect if it is absent.\n");
}

void case3() { // Testing free further. a) Does re-allocating give errors? b) Bounds-Checking if allocating at the end of the heap causes an error 
    char *ptr1 = malloc(4072);
    char *ptr2 = malloc(8);
    free(ptr2);
    char *ptr3 = malloc(8);
    if (ptr2 == ptr3) {
        printf("Test case 3 returns CORRECTLY\n");
    } else {
        printf("Test case 3 returns INCORRECTLY\n");
    }
    free(ptr3);
    free(ptr1);
}

void case4() { // Checking if alignment is done correctly by malloc
    int *ptr = malloc(87);
    
    if(get_payload_size(ptr) % 8 == 0) {
        printf("Test case 4 returns CORRECTLY\n");
    } else {
        printf("Test case 4 returns INCORRECTLY\n");
    }
}

void case5() { // Coalesce when the block to it's left is free
    int *ptr1 = malloc(16);
    int *ptr2 = malloc(24);
    int coalesced_size = 4088;
    free(ptr1);
    free(ptr2);
    if(get_payload_size(ptr1) == coalesced_size) {
        printf("Test case 5 returns CORRECTLY\n");
    } else {
        printf("Test case 5 returns INCORRECTLY\n");
    }
}

void case6() { // Coalesce when the block to the right is free
    int *ptr1 = malloc(16);
    int *ptr2 = malloc(24);
    int *ptr3 = malloc(32);
    int coalesced_size = get_payload_size(ptr1) + get_payload_size(ptr2) + 8;
    free(ptr2);
    
    free(ptr1);
    
    if(get_payload_size(ptr1) == coalesced_size) {
        printf("Test case 6 returns CORRECTLY\n");
    } else {
        printf("Test case 6 returns INCORRECTLY\n");
    }

    free(ptr3);
}

void case7() { // Coalesce when the block to the right and left is free
    int *ptr1 = malloc(16);
    int *ptr2 = malloc(24);
    int *ptr3 = malloc(32);
    int *ptr4 = malloc(40);
    int coalesced_size = get_payload_size(ptr1) + get_payload_size(ptr2) + get_payload_size(ptr3) + 16;
    free(ptr1);
    free(ptr3);

    free(ptr2);

    if(get_payload_size(ptr1) == coalesced_size) {
        printf("Test case 7 returns CORRECTLY\n");
    } else {
        printf("Test case 7 returns INCORRECTLY\n");
    }

    free(ptr4);
}

void case8() { // free should call run time error if freeing an unmalloced object
    char x;
    free(&x);
    printf("Test 8 is CORRECT if there is a *MEMORY ERROR* message above. It is incorrect if it is absent.\n");
}

void case9() {
    char *ptr = malloc(8);
    free(ptr+1);
    printf("Test 9 is CORRECT if there is a *MEMORY ERROR* message above. It is incorrect if it is absent.\n");
}

int main(int argc, char *argv[]){

    if(*argv[1] - '0' > 9 || *argv[1] - '0' < 1){
        printf("* INVALID INPUT *\n");
    }

    switch(*argv[1] - '0') {
        case 1:
            case1();
            break;
        case 2:
            case2();
            break;
        case 3:
            case3();
            break;
        case 4:
            case4();
            break;
        case 5:
            case5();
            break;
        case 6:
            case6();
            break;
        case 7:
            case7();
            break;
        case 8:
            case8();
            break;
        case 9:
            case9();
            break;
        
    }
    
    return 0;

}
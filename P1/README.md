Team Members:
Sibi suriyanarayan Tiruchirapalli venketaramani (st1005)
Rahulraj Rajesh (rr1185)

OUR CODE FILES:
1. mymalloc.c - contains our implementations of mymalloc, myfree, and helper functions (readheader, writeheader, isheader, get_payload_size)
2. mymalloc.h - contains our mymalloc, myfree initializations, get_payload_size initializations, their corresponding macros, and our header struct definition
3. memtest.c - contains a correctness test provided to us by Professor menendez
4. memgrind.c - contains our 5 performance tests
5. test.c - contains our 9 correctness tests
6. Makefile - used to compile our programs
7. README.txt - contains all the description and all implementation details of our programs and test cases.

HOW TO RUN OUR CODE:
1. Untar our project submission (tar -xf p1.tar)
2. Get into the correct directory (cd P1)
3. Compile our programs using the “makefile” (just simply call “make”)
4. Then use our macros to run our files (./mt OR ./g OR ./t)

Our Macros:
./mt (used to run memtest.c - takes 0 arguments)
./g (used to run memgrind.c - takes 0 arguments)
./t (used to run test.c - takes 1 argument) ← please note the 1 argument for test.c!!!!

To run memtest.c type in the terminal:
$make
$./mt

To run test.c type in the terminal:
$make
$./t n

IMPORTANT NOTE FOR TEST.C: the “n” is a number argument between (1-9) inclusive. Each number represents a correctness test case we have designed. We have a total of 9 correctness test cases in test.c

To run memgrind.c type in the terminal:
$make
$./g
IMPORTANT NOTE FOR MEMGRIND.C: the “n” is a number argument between (1-5) inclusive. Each number represents a performance test case we have designed. We have a total of 5 performance tests in memgrind.c


MAIN DESIGN NOTES:

We used a double array of size 512 for our memory and a header struct with a unsigned int called “size” and a short called “alloc_stat” (allocation_status). The way we used headers in our implementation was to create headers and write the byte data from the header to our memory array using pointer arithmetic. To read headers from the heap, write headers into the heap, get the payload size of any particular pointer, and other operations, we created helper functions for each operation.

MALLOC IMPLEMENTATION (mymalloc method in mymalloc.c):

malloc was implemented so that we could directly connect our header struct objects with the heap array. We often read and write the byte contents of our header structs to the heap array to “store” the header in the heap. 

- We intially bounds check the size so that any time the user provides a request to malloc 0 bytes or less, we invalidate the request. In our implementation, we considered mallocing 0 bytes as trivial and decided to disallow it. This is because, it would waste heap space by creating a header of size 8 bytes, with a payload size as 0.

- We have an initial check on whether or not a header was assigned initially to the heap array. This header should read "size = 4088 and alloc_stat = 0", which indicates that nothing has been allocated to memory yet (the heap is empty). On the first run of malloc we check to see if that aforementioned header is present. If it is not there, we make a header struct object and assign that header to the first 8 bytes of the heap array. Otherwise we move on.

- Next, we take the provided size to malloc (n) and align it to be an 8-byte payload. The new aligned variable is called "pl_size".

- Now for the bulk of our malloc function. We check through the heap array with the while loop, and read in the headers available in the heap array, going byte by byte. If the header reads to have an alloc_stat = 0 and size greater than pl_size, we allocate onto that free chunk. We additionally check if the remaining number of bytes is greater than or equal to 16. This is because the smallest allotment is 16 bytes (8 for the header, 8 for the payload). If there are enough bytes remaining in the heap, we make a new header for those remaining bytes. Otherwise we move on. Regardless, we then assign the current chunk with an allocation status of 1, marking it as “allocated”. Finally, through pointer arithmetic, we return the pointer to the first byte of the payload.

- In the event, the program exits the while loop, it means we have run out of memory. As a result we print the corresponding error with the source file and line number. Finally we return NULL, indicating a failed attempt to malloc.

FREE IMPLEMENTATION (myfree method in mymalloc.c):

free was implemented to sift through the heap array, find the header of the of the provided payload pointer, and set the alloc_stat of that header from “1” to “0” (AKA deallocated).

- To start off we initially try to locate the payload through the heap. We check byte by byte, comparing the address of the pointer to the address of the byte (because the pointer points to the first byte of the payload). When we have found it, we traverse back 8 bytes, because 8 bytes before the first byte of the payload is the first byte of the header. We read in the header into a header object, and break out of the loop.

- Outside of the for loop 4 have 2 run-time error checking conditions:
1. If the address of the pointer DOES NOT match the address of any of the heap array bytes. In the event, the loop never finds the byte matching the pointer’s address, we return an error. This is because it means that the user never called malloc on the pointer they are trying to free. 
2. If the address of the pointer DOES match the address of a heap byte. In this case the issue could be due to 3 things:
*** The user is trying to free a pointer that has already been freed (AKA double free). 
*** The user is trying to free a pointer to a byte in the heap, that has never been allocated before (In terms of the project, this error is hard to come by because there is really no way to have access to the bytes of the heap array unless you have access to mymalloc.c, but we tried to be comprehensive)
*** The user has a bad payload pointer. Suppose the user has the pointer to a payload that was called with malloc: *p. If they do pointer arithmetic and do free(p+1), that cannot stand. We cannot free part of the payload. So we call that out.

IMPORTANT NOTE: Error check 1 was made to check if the provided pointer’s address was never detected in the heap and the only reason that could happen is because they did not malloc yet. Error check 2, however, is made to check when they have a bad payload pointer. This error could happen for the 3 reasons mentioned above. In our implementation, we decided to not make separate error statements for those 3 reasons and combined them into one instead.

- If there are no headers we locate the previous header to our current header. We check if that previous header is free. If it is free, we coalesce with our current header to now have one giant free chunk. We use our “writeheader” helper method to write this new chunk to the heap. We also remove the current header since it has been coalesced and move on. If the previous header was not free, we simply move on.

- We the next header after our current header and check if it is free. If it is free, we update our current header to reflect the coalesced size and remove that next header. Otherwise we simply move on.

- Once we have our final chunk (coalesced if necessary), we set the alloc_stat parameter of the header to 0 and write this new final header to the heap. We simply return after that.

HELPER FUNCTIONS:


MEMTEST.C: 

- This file was provided by Professor Menendez. We did not touch this file at all. We included it in our project folder however, and our result ended up being “0 incorrect bytes”. Our actual Correctness Tests are included in a file called “test.c”. Please scroll to the section “HOW TO RUN OUR CODE” to learn about how to run this file.

CORRECTNESS TESTING (test.c):

- This file contains the entirety of our correctness tests. The main focus we had on this test was to test the requirements of the project and try to see if any of those requirements were violated. Please see our thought process further below. We had a total of 9 correctness test cases.

Case 1 (BLACK BOX TEST): Does malloc Allocate? Does malloc display run-time error correctly?
- To check if this works we basically called on malloc to make a chunk the entire heap size.
- If we were to malloc again on a new pointer object, that pointer object should be NULL. If it is not NULL, malloc has failed to allocate properly the first time.
- Additionally, we also check for the presence of an error message. Is our code actually able to catch the over-allocating error?
- By calling malloc on the entire heap and then calling malloc again without freeing, we ensure that we test if our malloc works and if we receive the correct run-time error messages.

Case 2 (BLACK BOX TEST): Does free deallocate? Does freeing it from the beginning of memory cause any bounds errors? Does free display the run-time error correctly?
- We malloced an arbitrary number of bytes. Then we freed the pointer twice
- In the event that no error message was returned, our free is not working. This is because you cannot free memory that has already been deallocated, which is exactly what we are doing.
- Additionally, we are freeing the very first pointer we assign, and therefore checking if that throws any errors.
- If the case shows that an error is present from freeing twice, it means that our free works fine!
- This case is able to test all of those errors simply by freeing an allocated pointer twice.

Case 3 (BLACK BOX TEST): Does reallocating give errors? Does allocating at the end of memory give errors?
- We malloc all of memory except the last 16 bytes
- For the last 16, we have another allocation
- We free that and then reallocate on the same spot
- If malloc and free don’t interfere with each other we have success. Additionally, we have further success if allocating and freeing at the end of memory have no impact on our program.

Case 4 (WHITE BOX TEST): Checking if alignment is done correctly
- The project requires 8-byte alignment. We malloc a number of bytes not divisible by 8.
- Using a helper function in mymalloc.c called “get_payload_size”, we see if the associated payload size after mallocing is divisible by 8
- Testing this directly shows us if we meet 8-byte alignment criteria

Case 5 (WHITE BOX TEST): Test coalescing again
- If the block to the left is free, does the heap coalesce correctly?
- If we assign 2 pointers, and then free them, the coalesced size should be the size of heap minus the header-size (4088 bytes)
- We use the “get-payload-size” method on ptr1 to test this, since it is the first ptr allocated to the heap

Case 6 (WHITE BOX TEST): Test coalescing further
- If the block to the right is free, does the heap coalesce correctly?
- If we assign 3 pointers, free the 2nd assigned one, then free the first one, the coalesced size should be payload size of ptr1 + payload size of ptr2 + the headersize (between both pointers).
- We use the “get-payload-size” method on ptr1 again to test this

Case 7 (WHITE BOX TEST): Test coalescing even further
- If the block to the right AND LEFT iare free, does the heap coalesce correctly?
- If we assign 4 pointers, free the 3rd assigned one, then free the first one,and then finally free the 2nd one, the coalesced size should be payload size of ptr1 + payload size of ptr2 + payload size of ptr3 + 2 * the headersize (because 2 headers between 3 pointers).
- We use the “get-payload-size” method on ptr1 again to test this

Case 8 (BLACK BOX TEST): Does freeing an unallocated object call the run-time error?
- If we have a regular object uninitialized by malloc, freeing it should print out an error
- This is because we have not actually made the space for the object in the heap and so calling free on such an object is an impossible request
- If an error message appears, our test runs as it should

Case 9 (BLACK BOX TEST): If we malloc an object and call free on the 2nd byte (or any byte besides the 1st) on the payload pointer, do we receive an error?
- In other words, if we are freeing an address not at the start of the chunk (particularly the payload), we should be able to free that.
- By testing it on another byte besides the starting byte of the payload, we can check to see if we receive a valid error
- If we do not, our test has failed, otherwise we have succeeded.

PERFORMANCE TESTING (memgrind.c):
The performance tests that we used memgrind are as follows:

Test 1: Call malloc() and then free(), 120 times
- Tests to see how fast calling malloc() and then free() is 120 times
- Average time: 0.001011 seconds

Test 2: Call malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
- Tests to see how fast using malloc() to make 1-byte objects takes 120 times, and then freeing the 120 allocated chunks after
- Average time: 0.001233 seconds

Test 3: Create an array of 120 pointers and make a random choice between malloc(1) and deallocating a previously allocated object (if any)
- Tests to see how fast malloc() and free() work randomly, 120 times
- Average time: 0.001024 seconds
- NOTE: Errors will occur during the execution of this test, which are normal.

Test 4: Call malloc() 120 times, free() 60 times, malloc() 196 times, and free() 256 times
- Tests to see how fast calling malloc() and free() work for a certain number of times until the entire memory array is allocated, and then freeing all of the chunks
- Average time: 0.003697 seconds

Test 5: Call malloc() 256 times, free() even indices, and free() odd indices
- Tests to see how fast calling malloc() 256 times executes, as well as freeing the even chunks and then freeing the odd chunks.
- Average time: 0.002882 seconds




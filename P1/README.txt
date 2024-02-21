Project 1: My Little malloc()

Team Members:
Sibi Suriyanarayan Tiruchirapalli Venketaramani (st1005)
Rahulraj Rajesh (rr1185)

Design Notes:

We used a double pointer array of size 512 for our memory and a header struct with a unsigned int size variable and a 
short alloc_stat (allocation_status) variable. The way we used headers in our implementation was to create headers and 
write the byte data from the header to our memory array using pointer arithmetic. To read headers, write headers, get
the payload size and other operations, we created functions for each operation.

** malloc implementation: malloc was implemented so that we could directly connect our header struct objects with the heap
array. 
- We intiially bounds check the size so that any time the user provides a request to malloc 0 bytes or less, we invalidate
the error. In our implementation, we considered mallocing 0 bytes as trivial and decided to disallow it. This is because, it would
waste space by creating a header of size 8 bytes, with a payload size as 0.
- On the first implementation of 

** free implementation: free was implemented

Makefile (skullemoji):

Memtest:

Memgrind:
The performance tests that we used memgrind are as follows:
1. malloc() and immediately free() a 1-byte object, 120 times.
2. Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
 * 3. Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), 
 *    until you have allocated 120 times. Deallocate any remaining objects.
 * - Keep a count of how many tings have been allocated by malloc
 * - If count == 0 and you get a free --> let the error happen
 * - If count == n, where n != 0 and you get a free --> free(arr[n-1])
 * - Free any remaining ptrs
 * 4. Malloc 120 times, free 60 times, malloc 240 times, free 300 times;
 * 5. Malloc 256 times, free even indices, free odd indices
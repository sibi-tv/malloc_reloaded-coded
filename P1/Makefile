all: memtest memgrind test

memtest:
	gcc -Wall memtest.c mymalloc.c -o mt

memgrind:
	gcc -Wall memgrind.c mymalloc.c -o g

test:
	gcc -Wall test.c mymalloc.c -o t

clean:
	rm -rf t g mt
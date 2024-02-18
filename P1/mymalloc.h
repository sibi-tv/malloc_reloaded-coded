#ifndef _MYMALLOC_H
#define _MYMALLOC_H

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

struct {
    unsigned int size; // Equals the requested size + closest 8
    short alloc_stat;
} typedef header;

#endif

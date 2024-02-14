#include <stdlib.h>

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE_)

struct {
    unsigned int size; // The size is header size (8 bytes) + payload size (requested n bytes)
    short alloc_stat;
} typedef chunk;

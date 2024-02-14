#include <stdlib.h>

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE_)

struct {
    size_t byte_length; // The size is header size (8 bytes) + payload size (requested n bytes)
    unsigned char allocation_status; // is either a 0 or a 1. Throw error if other number??
} typedef chunk;
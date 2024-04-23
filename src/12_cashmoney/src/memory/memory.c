#include "kmalloc.h"

void *malloc(size_t size)
{
    return kmalloc(size);
}

void free(void *mem)
{
    kfree(mem);
}
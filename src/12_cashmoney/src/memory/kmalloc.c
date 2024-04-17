#include "kmalloc.h"
#include <libc/stdbool.h>

typedef struct
{
    uint32_t last_alloc;
    uint32_t heap_begin, heap_end;
    uint32_t page_heap_begin, page_heap_end;
    uint32_t *page_head_desc;
    uint32_t mem_used;
}kmalloc_data;

void init_kernel_memory(uint32_t* kernel_end)
{
    //
}
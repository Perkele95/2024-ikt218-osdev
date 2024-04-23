#include "kmalloc.h"
#include <libc/stdbool.h>
#include "memutils.h"
#include "../monitor/monitor.h"

#define MEM_PAGE_SIZE 0x1000
#define MEM_PAGE_HEAP_END 0x400000
#define MEM_PAGE_MAX_ALIGNED_ALLOCS 32
#define MEM_ALLOC_PADDING 0x4

typedef struct
{
    uint32_t last_alloc;
    uint32_t begin, end;
    uint32_t page_begin, page_end;
    bool *page_desc;
    uint32_t mem_used;
}memory_data_t;

typedef struct
{
    bool status;
    size_t size;
}alloc_t;

static memory_data_t Memory = {0};

void print_memory_layout()
{
    monitor_write("Memory used: ");
    monitor_write_udec(Memory.mem_used);
    monitor_write(" bytes\nMemoryfree: ");
    monitor_write_udec(Memory.end - Memory.begin - Memory.mem_used);
    monitor_write(" bytes\nKernel memory start: ");
    monitor_write_hex(Memory.begin);
    monitor_write("\nKernel memory end: ");
    monitor_write_hex(Memory.end);
    monitor_write("\nKernel memory size: ");
    monitor_write_udec(Memory.end - Memory.begin);
    monitor_write(" bytes\nPaging memory start: ");
    monitor_write_hex(Memory.page_begin);
    monitor_write("\nPaging memory end: ");
    monitor_write_hex(Memory.page_end);
    monitor_write("\nPaging memory size: ");
    monitor_write_udec(Memory.page_end - Memory.page_begin);
    monitor_write("bytes \n");
}

void print_alloc(alloc_t *alloc)
{
    monitor_write("Allocated ");
    monitor_write_udec(alloc->size);
    monitor_write(" bytes from ");
    monitor_write_hex((uint32_t)(alloc + 1));
    monitor_write(" to ");
    monitor_write_hex((uint32_t)((uint8_t*)(alloc + 1) + alloc->size));
    monitor_put('\n');
}

void *kmalloc(size_t size)
{
    if(size == 0)
        return 0;

    uint8_t *mem = (uint8_t*)Memory.begin;

    while((uint32_t)mem < Memory.last_alloc)
    {
        alloc_t *alloc = (alloc_t*)mem;

        // Unused section found, break
        if(alloc->size == 0)
            break;

        // If alloc is used, move past it
        if(alloc->status)
        {
            mem += alloc->size + sizeof(alloc_t) + MEM_ALLOC_PADDING;
            continue;
        }

        // Allocate and adjust if block can contain the requested size
        if(alloc->size >= size)
        {
            alloc->status = true;
            uint8_t *alloc_start = mem + sizeof(alloc_t);
            Memory.mem_used += size + sizeof(alloc_t);
            memset(alloc_start, 0, size);
            print_alloc(alloc);
            return alloc_start;
        }

        // If not, move to next block
        mem += alloc->size + sizeof(alloc_t) + MEM_ALLOC_PADDING;
    }

    if(Memory.last_alloc + size + sizeof(alloc_t) >= Memory.end)
    {
        monitor_write("!FATAL! Out of kernel memory");
        return 0;
    }

    alloc_t *alloc = (alloc_t*)Memory.last_alloc;
    alloc->status = true;
    alloc->size = size;

    Memory.last_alloc += size + sizeof(alloc_t) + MEM_ALLOC_PADDING;
    Memory.mem_used += size + sizeof(alloc_t) + MEM_ALLOC_PADDING;
    memset((uint8_t*)alloc + sizeof(alloc_t), 0, size);
    print_alloc(alloc);
    return (uint8_t*)alloc + sizeof(alloc_t);
}

void *kmalloc_aligned(size_t size)
{
    // Find next available page
    for (size_t i = 0; i < MEM_PAGE_MAX_ALIGNED_ALLOCS; i++)
    {
        if(!Memory.page_desc[i])
        {
            Memory.page_desc[i] = true;
            const uint32_t address = Memory.page_begin + i * MEM_PAGE_SIZE;
            monitor_write("Align allocated from ");
            monitor_write_hex(address);
            monitor_write(" to ");
            monitor_write_hex(address + MEM_PAGE_SIZE);
            monitor_put('\n');
            return (void*)address;
        }
    }

    monitor_write("!FATAL! Page allocation failed: out of pages\n");
    return 0;
}

void kfree(void *mem)
{
    alloc_t *alloc = (alloc_t*)mem - 1;
    Memory.mem_used -= alloc->size + sizeof(alloc_t);
    alloc->status = false;
}

void kfree_aligned(void *mem)
{
    const uint32_t address = (uint32_t)mem;
    // Check if address is within bounds of page memory
    if(address > Memory.page_begin && address < Memory.page_end)
    {
        // Calculate page id
        uint32_t page_id = address;
        page_id -= Memory.page_begin;
        page_id /= MEM_PAGE_SIZE;

        Memory.page_desc[page_id] = false;
    }
}

void init_kernel_memory(uint32_t *kernel_end)
{
    Memory.last_alloc = (uint32_t)(kernel_end + MEM_PAGE_SIZE);
    Memory.page_end = MEM_PAGE_HEAP_END;
    Memory.page_begin = MEM_PAGE_HEAP_END - (MEM_PAGE_MAX_ALIGNED_ALLOCS * MEM_PAGE_SIZE);
    Memory.begin = Memory.last_alloc;
    Memory.end = Memory.page_begin;
    memset((void*)Memory.begin, 0, Memory.end - Memory.begin);
    Memory.page_desc = (bool*)kmalloc(MEM_PAGE_MAX_ALIGNED_ALLOCS);
}

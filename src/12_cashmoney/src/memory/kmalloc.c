#include "kmalloc.h"
#include <libc/stdbool.h>

uint32_t placement_address = 0;

void init_kernel_memory(uint32_t *pEnd)
{
    placement_address = *pEnd;
}

static uint32_t kmalloc_internal(uint32_t size, bool align, uint32_t *phys)
{
    // Align memory to a page
    if (align && (placement_address & 0xFFFFF000))
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    if(phys)
        *phys = placement_address;

    const uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

uint32_t kmalloc(uint32_t size)
{
    return kmalloc_internal(size, false, 0);
}

uint32_t kmalloc_aligned(uint32_t size)
{
    return kmalloc_internal(size, true, 0);
}

uint32_t kmalloc_phys(uint32_t size, uint32_t *phys)
{
    return kmalloc_internal(size, false, phys);
}

uint32_t kmalloc_phys_aligned(uint32_t size, uint32_t *phys)
{
    return kmalloc_internal(size, true, phys);
}
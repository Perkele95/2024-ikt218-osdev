#include "kmalloc.h"
#include <libc/stdbool.h>

uint32_t PlacementAddress = 0;

static uint32_t kmalloc_internal(uint32_t size, bool align, uint32_t *phys)
{
    // Align memory to a page
    if (align && (PlacementAddress & 0xFFFFF000))
    {
        PlacementAddress &= 0xFFFFF000;
        PlacementAddress += 0x1000;
    }

    if(phys)
        *phys = PlacementAddress;

    const uint32_t tmp = PlacementAddress;
    PlacementAddress += size;
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
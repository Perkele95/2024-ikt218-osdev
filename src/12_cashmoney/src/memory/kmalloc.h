#pragma once

#include <libc/stdint.h>

void init_kernel_memory(uint32_t *pEnd);
uint32_t kmalloc(uint32_t size);
uint32_t kmalloc_aligned(uint32_t size);
uint32_t kmalloc_phys(uint32_t size, uint32_t *phys);
uint32_t kmalloc_phys_aligned(uint32_t size, uint32_t *phys);
#pragma once

#include <libc/stdint.h>

uint32_t kmalloc(uint32_t size);
uint32_t kmalloc_aligned(uint32_t size);
uint32_t kmalloc_phys(uint32_t size, uint32_t *phys);
uint32_t kmalloc_phys_aligned(uint32_t size, uint32_t *phys);
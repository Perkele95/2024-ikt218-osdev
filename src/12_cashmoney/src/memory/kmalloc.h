#pragma once

#include "../common.h"

void print_memory_layout();
void *kmalloc(size_t size);
void *kmalloc_aligned(size_t size);
void kfree(void *mem);
void kfree_aligned(void *mem);

void init_kernel_memory(uint32_t *kernel_end);
#pragma once

#include "../common.h"
#include "kmalloc.h"

#define PAGE_TABLE_SIZE 1024

typedef struct
{
    uint32_t present : 1;
    uint32_t rw : 1; // Read-write
    uint32_t user : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t reserved : 7; // Reserved for
    uint32_t frame : 20; // Frame address
}page_t;

typedef struct
{
    // Pointer to array of arrays of pages
    page_t *pTables[PAGE_TABLE_SIZE][PAGE_TABLE_SIZE];

    // Physical address of tables
    uint32_t tablesPhysical[PAGE_TABLE_SIZE];

    // Physical address of tablesPhysical
    uint32_t physicalAddress;
}page_directory_t;

void init_paging();

#include "paging.h"
#include "kmalloc.h"

#define PAGES_SECTION_SIZE 0x400000
#define PAGES_DIR_ADDRESS 0x400000 // Set directory to start at 4mb
#define PAGES_LAST_PAGE_OFFSET 0x4000 // Set last page to end 4kb after start
#define PAGES_PAGE_COUNT 1024
#define PAGES_PAGE_SIZE 4096

#define PAGES_PRESENT_BIT 1
#define PAGES_RW_BIT (1 << 1)
#define PAGES_USER_MODE_BIT (1 << 2)

typedef struct
{
    uint32_t *directory;
    uint32_t *last_page;
    uint32_t dir_address;
}page_data_t;

static page_data_t Pages = {0};

void paging_map_virtual_to_physical(uint32_t virtual, uint32_t physical)
{
    for (size_t i = 0; i < PAGES_PAGE_COUNT; i++)
    {
        Pages.last_page[i] = physical | PAGES_PRESENT_BIT | PAGES_RW_BIT;
        physical += PAGES_PAGE_SIZE;
    }

    // 22 because we want the upper 10 bits (32-10 = 22)
    const uint32_t id = virtual >> 22;
    Pages.directory[id] = ((uint32_t)Pages.last_page) | PAGES_PRESENT_BIT | PAGES_RW_BIT;
    Pages.last_page = (uint32_t*)(((uint32_t)Pages.last_page) + PAGES_PAGE_SIZE);
}

void enable_paging()
{
    // Load phys address to cr3 register
    asm volatile("mov %%eax, %%cr3": :"a"(Pages.dir_address));
    // Put content of cr0 into eax
    asm volatile("mov %cr0, %eax");
    // Set paging-enable bit in eax register
    asm volatile("orl $0x80000000, %eax");
    // Put content of eax into cr0
    asm volatile("mov %eax, %cr0");
}

void init_paging()
{
    // Set page locations
    Pages.directory = (uint32_t*)PAGES_DIR_ADDRESS;
    Pages.last_page = (uint32_t*)(PAGES_DIR_ADDRESS + PAGES_LAST_PAGE_OFFSET);
    Pages.dir_address = PAGES_DIR_ADDRESS;

    // Init page bits
    for (size_t i = 0; i < PAGES_PAGE_COUNT; i++)
        Pages.directory[i] = PAGES_USER_MODE_BIT; // Set pages to user mode

    // Map first 4mb of virtual to physical memory
    paging_map_virtual_to_physical(0, 0);
    // Map next 4mb of virtual to physical memory
    paging_map_virtual_to_physical(PAGES_SECTION_SIZE, PAGES_SECTION_SIZE);
    enable_paging();
}
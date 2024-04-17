#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include <multiboot2.h>
#include "descriptor_tables/descriptor_tables.h"
#include "monitor/monitor.h"
#include "interrupt/interrupt.h"
#include "input/input.h"

extern uint32_t end; // This is defined in arch/i386/linker.ld

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

void page_fault_test()
{
    uint32_t *ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{
    init_descriptor_tables();
    init_keyboard_input();
#if 0
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();
#endif
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}
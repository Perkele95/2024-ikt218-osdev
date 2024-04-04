#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include <multiboot2.h>
#include "descriptor_tables/descriptor_tables.h"
#include "monitor/monitor.h"
#include "interrupt/interrupt.h"
#include "input/input.h"

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{
    init_descriptor_tables();
    init_keyboard_input();

    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}
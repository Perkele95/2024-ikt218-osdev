#include <libc/limits.h>
#include "descriptor_tables.h"

// Access to ASM functions
extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

gdt_entry_t gdt_entries[GDT_ENTRY_COUNT];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[IDT_ENTRY_COUNT];
idt_ptr_t   idt_ptr;

static void gdt_set(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    // Shifts and truncates the 32 bit base value to fit into the 3 partitions
    gdt_entries[index].base_low    = (base & 0xFFFF);
    gdt_entries[index].base_middle = (base >> 16) & 0xFF;
    gdt_entries[index].base_high   = (base >> 24) & 0xFF;

    // Truncates limit
    gdt_entries[index].limit_low   = (limit & 0xFFFF);

    // Sets half of granularity from the upper part of limit
    gdt_entries[index].granularity = (limit >> 16) & 0x0F;

    // Sets the upper half of granularity from gran
    gdt_entries[index].granularity |= gran & 0xF0;
    gdt_entries[index].access      = access;
}

static void init_gdt()
{
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    gdt_set(0, 0, 0, 0, 0);               // Null descriptor segment
    gdt_set(1, 0, UINT32_MAX, 0x9A, 0xCF); // Kernel code segment
    gdt_set(2, 0, UINT32_MAX, 0x92, 0xCF); // Kernel data segment
    gdt_set(3, 0, UINT32_MAX, 0xFA, 0xCF); // User mode code segment
    gdt_set(4, 0, UINT32_MAX, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t)&gdt_ptr);
}

static void idt_set(uint8_t index, uint32_t base, uint16_t sel, uint8_t flags, int mode)
{
    idt_entries[index].base_low = base & UINT16_MAX;
    idt_entries[index].base_high = (base >> 16) & UINT16_MAX;

    idt_entries[index].select = sel;
    idt_entries[index].always0 = 0;
    // This sets privilege level lower for user mode
    idt_entries[index].flags = mode == KERNEL_MODE ? flags : flags | 0x60;
}

static void remap_irq_table()
{
    outbyte(PIC1, ICW1_INIT | ICW1_ICW4);
    outbyte(PIC2, ICW1_INIT | ICW1_ICW4);
    outbyte(PIC1_DATA, 0x20);
    outbyte(PIC2_DATA, 0x28);
    outbyte(PIC1_DATA, 0x04);
    outbyte(PIC2_DATA, 0x02);
    outbyte(PIC1_DATA, ICW4_8086);
    outbyte(PIC2_DATA, ICW4_8086);
    outbyte(PIC1_DATA, 0x0);
    outbyte(PIC2_DATA, 0x0);
}

static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entries) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entries));

    remap_irq_table();

    // ISRs
    idt_set(0, (uint32_t)isr0, 0x08, 0x8E, KERNEL_MODE);
    idt_set(1, (uint32_t)isr1, 0x08, 0x8E, KERNEL_MODE);
    idt_set(2, (uint32_t)isr2, 0x08, 0x8E, KERNEL_MODE);
    idt_set(3, (uint32_t)isr3, 0x08, 0x8E, KERNEL_MODE);
    idt_set(4, (uint32_t)isr4, 0x08, 0x8E, KERNEL_MODE);
    idt_set(5, (uint32_t)isr5, 0x08, 0x8E, KERNEL_MODE);
    idt_set(6, (uint32_t)isr6, 0x08, 0x8E, KERNEL_MODE);
    idt_set(7, (uint32_t)isr7, 0x08, 0x8E, KERNEL_MODE);
    idt_set(8, (uint32_t)isr8, 0x08, 0x8E, KERNEL_MODE);
    idt_set(9, (uint32_t)isr9, 0x08, 0x8E, KERNEL_MODE);
    idt_set(10, (uint32_t)isr10, 0x08, 0x8E, KERNEL_MODE);
    idt_set(11, (uint32_t)isr11, 0x08, 0x8E, KERNEL_MODE);
    idt_set(12, (uint32_t)isr12, 0x08, 0x8E, KERNEL_MODE);
    idt_set(13, (uint32_t)isr13, 0x08, 0x8E, KERNEL_MODE);
    idt_set(14, (uint32_t)isr14, 0x08, 0x8E, KERNEL_MODE);
    idt_set(15, (uint32_t)isr15, 0x08, 0x8E, KERNEL_MODE);
    idt_set(16, (uint32_t)isr16, 0x08, 0x8E, KERNEL_MODE);
    idt_set(17, (uint32_t)isr17, 0x08, 0x8E, KERNEL_MODE);
    idt_set(18, (uint32_t)isr18, 0x08, 0x8E, KERNEL_MODE);

    // IRQs
    idt_set(32, (uint32_t)irq0, 0x08, 0x8E, KERNEL_MODE);
    idt_set(33, (uint32_t)irq1, 0x08, 0x8E, KERNEL_MODE);
    idt_set(34, (uint32_t)irq2, 0x08, 0x8E, KERNEL_MODE);
    idt_set(35, (uint32_t)irq3, 0x08, 0x8E, KERNEL_MODE);
    idt_set(36, (uint32_t)irq4, 0x08, 0x8E, KERNEL_MODE);
    idt_set(37, (uint32_t)irq5, 0x08, 0x8E, KERNEL_MODE);
    idt_set(38, (uint32_t)irq6, 0x08, 0x8E, KERNEL_MODE);
    idt_set(39, (uint32_t)irq7, 0x08, 0x8E, KERNEL_MODE);
    idt_set(40, (uint32_t)irq8, 0x08, 0x8E, KERNEL_MODE);
    idt_set(41, (uint32_t)irq9, 0x08, 0x8E, KERNEL_MODE);
    idt_set(42, (uint32_t)irq10, 0x08, 0x8E, KERNEL_MODE);
    idt_set(43, (uint32_t)irq11, 0x08, 0x8E, KERNEL_MODE);
    idt_set(44, (uint32_t)irq12, 0x08, 0x8E, KERNEL_MODE);
    idt_set(45, (uint32_t)irq13, 0x08, 0x8E, KERNEL_MODE);
    idt_set(46, (uint32_t)irq14, 0x08, 0x8E, KERNEL_MODE);
    idt_set(47, (uint32_t)irq15, 0x08, 0x8E, KERNEL_MODE);

    idt_flush((uint32_t)&idt_ptr);
}

void init_descriptor_tables()
{
    init_gdt();
    init_idt();
}
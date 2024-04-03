#include "isr.h"
#include <libc/stdio.h>
#include <libc/string.h>

#define IRQ_RESET_SIGNAL 0x20

isr_t interrupt_handlers[IDT_ENTRY_COUNT];

void set_interrupt_handler(size_t n, isr_t fn)
{
    interrupt_handlers[n] = fn;
}

const char *get_interrupt_string(uint32_t interrupt)
{
    static const char *s_InterruptString[] =
    {
        "Division by zero exception",
        "Debug exception",
        "Non maskable interrupt",
        "Breakpoint exception",
        "Into detected overflow'",
        "Out of bounds exception",
        "Invalid opcode exception",
        "No coprocessor exception",
        "Double fault",
        "Coprocessor segment overrun",
        "Bad TSS",
        "Segment not present",
        "Stack fault",
        "General protection fault",
        "Page fault",
        "Unknown interrupt exception",
        "Coprocessor fault",
        "Alignment check exception",
        "Machine check exception"
    };

    if(interrupt > 18)
    {
        return "Reserved";
    }

    return s_InterruptString[interrupt];
}

void isr_handler(registers_t regs)
{
    monitor_write("Interrupt: ");
    monitor_write(get_interrupt_string(regs.int_no));
    monitor_put('\n');
}

// Receives incoming interrupts and forwards the call
void irq_handler(registers_t regs)
{
    // Slave PIC pins go from 40->47
    if (regs.int_no >= 40)
    {
        outbyte(PIC2, IRQ_RESET_SIGNAL);
    }

    outbyte(PIC1, IRQ_RESET_SIGNAL);

    // Ensure the handler exists before calling
    if (interrupt_handlers[regs.int_no] != 0)
    {
        interrupt_handlers[regs.int_no](regs);
    }
}
#pragma once

#include <libc/stdint.h>

#define ISR0 0 // Division by zero exception
#define ISR1 1 // Debug exception
#define ISR2 2 // Non maskable interrupt
#define ISR3 3 // Breakpoint exception
#define ISR4 4 // 'Into detected overflow'
#define ISR5 5 // Out of bounds exception
#define ISR6 6 // Invalid opcode exception
#define ISR7 7 // No coprocessor exception
#define ISR8 8 // Double fault (pushes an error code)
#define ISR9 9 // Coprocessor segment overrun
#define ISR10 10 // Bad TSS (pushes an error code)
#define ISR11 11 // Segment not present (pushes an error code)
#define ISR12 12 // Stack fault (pushes an error code)
#define ISR13 13 // General protection fault (pushes an error code)
#define ISR14 14 // Page fault (pushes an error code)
#define ISR15 15 // Unknown interrupt exception
#define ISR16 16 // Coprocessor fault
#define ISR17 17 // Alignment check exception
#define ISR18 18 // Machine check exception

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// IRQ values, from OSDev wiki
#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_DATA (PIC1+1)
#define PIC2_DATA (PIC2+1)
#define ICW1_ICW4 0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE 0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08		/* Level triggered (edge) mode */
#define ICW1_INIT 0x10		/* Initialization - required! */
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define GDT_ENTRY_COUNT 5
#define IDT_ENTRY_COUNT 256

#define array_size(array) ((sizeof(array)) / (sizeof(array[0])))

// Write a byte to the specified port
static inline void outbyte(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Read a byte from the specified port
static inline uint8_t inbyte(uint16_t port)
{
   uint8_t ret = 0;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

// Read a word from the specified port
static inline uint16_t inword(uint16_t port)
{
   uint16_t ret = 0;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

// Absolute value
static inline int32_t abs32(int32_t value)
{
    return (value < 0 ? -value : value);
}
// Absolute value
static inline int16_t abs16(int16_t value)
{
    return (value < 0 ? -value : value);
}
// Absolute value
static inline int8_t abs8(int8_t value)
{
    return (value < 0 ? -value : value);
}
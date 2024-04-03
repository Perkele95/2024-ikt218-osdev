#pragma once

#include <libc/stdint.h>

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

#define PIT_CH0 0x40
#define PIT_CH1 0x41
#define PIT_CH2 0x42
#define PIT_COMMAND 0x43

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

// Length is in bytes
static inline void *memset(void *dst, int value, int len)
{
    uint8_t *ptr = dst;

    while(len > 0)
    {
        *(ptr++) = value;
        len--;
    }

    return dst;
}
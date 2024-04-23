#include "pit.h"
#include "../interrupt/interrupt.h"
#include "../monitor/monitor.h"

#define PIT_CH0 0x40
#define PIT_CH1 0x41
#define PIT_CH2 0x42
#define PIT_COMMAND 0x43
#define PIC_EOI 0x20 /* End-of-interrupt command code */

#define PIT_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000
#define PIT_DIVISOR (PIT_FREQUENCY / TARGET_FREQUENCY)

static volatile uint32_t Tick = 0;

void timer_fn(registers_t regs)
{
    if(Tick)
        Tick--;
}

void init_timer(uint32_t frequency)
{
    const uint32_t divisor = PIT_FREQUENCY / frequency;

    // send command byte
    outbyte(PIT_COMMAND, 0x36);

    // Send the frequency divisor, bytewise
    outbyte(PIT_CH0, (uint8_t)(divisor & 0xFF));
    outbyte(PIT_CH0, (uint8_t)((divisor >> 8) & 0xFF));
}

void init_pit()
{
    set_interrupt_handler(IRQ0, &timer_fn);
    init_timer(TARGET_FREQUENCY);
}

void sleep_interrupt(uint32_t milliseconds)
{
    Tick = milliseconds;
    while(Tick)
    {
        asm volatile("hlt");
    }
}

void sleep_busy(uint32_t milliseconds)
{
    Tick = milliseconds;
    while(Tick)
    {
        // Do nothing
    }
}
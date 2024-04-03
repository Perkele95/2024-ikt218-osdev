#include "timer.h"
#include "../interrupt/isr.h"
#include "../monitor/monitor.h"

#define TIMER_FREQUENCY 1193180

static uint32_t tick = 0;

void timer_fn(registers_t regs)
{
    monitor_write("Tick: ");
    monitor_write_udec(++tick);
    monitor_put('\n');
}

void init_timer(uint32_t frequency)
{
    set_interrupt_handler(IRQ0, &timer_fn);

    uint32_t divisor = TIMER_FREQUENCY / frequency;

    // send command byte
    outbyte(PIT_COMMAND, 0x36);

    // Send the frequency divisor, bytewise
    outbyte(PIT_CH0, (uint8_t)(divisor & 0xFF));
    outbyte(PIT_CH0, (uint8_t)((divisor >> 8) & 0xFF));
}
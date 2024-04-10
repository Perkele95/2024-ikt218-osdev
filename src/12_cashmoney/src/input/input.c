#include "input.h"
#include "../interrupt/interrupt.h"
#include "../monitor/monitor.h"

#define PS2_DATA 0x60
#define SCANCODE_KEY_RELEASE_BIT 0x80

enum SCANCODE_CONTROL
{
    SCANCODE_CONTROL_LCTRL = 0x2A,
    SCANCODE_CONTROL_RCTRL = 0x36,
};

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

static bool ShiftActive = false;

void handle_key_press(uint8_t scancode)
{
    if(scancode == SCANCODE_CONTROL_LCTRL || scancode == SCANCODE_CONTROL_RCTRL)
        ShiftActive = true;
    else if(ShiftActive)
        monitor_put(kbdus[scancode] - (char)32);
    else
        monitor_put(kbdus[scancode]);
}

void handle_key_release(uint8_t scancode)
{
    if(scancode == SCANCODE_CONTROL_LCTRL || scancode == SCANCODE_CONTROL_RCTRL)
        ShiftActive = false;
}

void keyboard_callback(registers_t regs)
{
    const uint8_t scancode = inbyte(PS2_DATA);

    if(scancode & SCANCODE_KEY_RELEASE_BIT)
    {
        // Release :: truncate to exclude release bit
        const uint8_t scan = scancode & (SCANCODE_KEY_RELEASE_BIT - 1);
        handle_key_release(scan);
    }
    else
    {
        // Press
        handle_key_press(scancode);
    }
}

void init_keyboard_input()
{
    set_interrupt_handler(IRQ1, keyboard_callback);
}
#pragma once

#include "../common.h"

typedef struct
{
   uint32_t ds; // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
}registers_t;

typedef void (*isr_t)(registers_t);

void set_interrupt_handler(size_t n, isr_t fn);
#pragma once

#include <libc/stdint.h>

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
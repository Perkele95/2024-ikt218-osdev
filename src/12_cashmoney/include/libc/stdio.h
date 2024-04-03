#pragma once

#include <libc/stdint.h>
#include <libc/stdbool.h>

int putchar(int c);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);
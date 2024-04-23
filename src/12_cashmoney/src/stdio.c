#include <libc/stdio.h>
#include <libc/stdarg.h>
#include "monitor/monitor.h"

void print_char(int c)
{
    monitor_put(c);
}

static char *convert(uint32_t number, uint32_t base)
{
    static char Rep[] = "0123456789ABCDEF";
    static char Buffer[50] = {0};

    // Prevent base higher than 16
    if(base > 16)
        base = 16;

    char *ptr = &Buffer[sizeof(Buffer) - 1];
    *ptr = '\0';

    do
    {
        *(--ptr) = Rep[number % base];
        number /= base;
    } while (number != 0);

    return ptr;
}

void print_fmt(const char* __restrict__ format, ...)
{
    const char *c = 0;
    int i = 0;

    // Args
    va_list arg;
    va_start(arg, format);

    for (c = format; *c != '\0'; c++)
    {
        if(*c != '%')
        {
            print_char(*c);
        }
        else
        {
            c++;
            switch (*c)
            {
                case 'c':{
                    i = va_arg(arg, int);
                    print_char(i);
                    break;
                }
                case 'i':
                case 'd':{
                    i = va_arg(arg, int);
                    if(i < 0)
                    {
                        i = -i;
                        print_char('-');
                    }
                    monitor_write(convert(i, 10));
                    break;
                }
                case 'o':{
                    i = va_arg(arg, unsigned int);
                    monitor_write(convert(i, 8));
                    break;
                }
                case 's':{
                    char *s = va_arg(arg, char*);
                    monitor_write(s);
                    break;
                }
                case 'x':{
                    i = va_arg(arg, unsigned int);
                    monitor_write(convert(i, 16));
                    break;
                }
                default: break;
            }
        }
    }

    // Close args
    va_end(arg);
}
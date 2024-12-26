#ifndef __PRINTF_H__
#define __PRINTF_H__

#include <dev/cereal.h>
#include <KrnlAid/utils/nanoprintf.h>
#include <stdarg.h>

extern void print_char(int c, void *ctx);

static inline void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    npf_vpprintf(print_char, 0, fmt, args);
    va_end(args);
}
#endif // __PRINTF_H__
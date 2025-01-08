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

static inline void kvprintf(const char *fmt, va_list args) {
    npf_vpprintf(print_char, 0, fmt, args);
}

static inline void ksnprintf(char *buffer, size_t bufsz, char const *fmt, ...) {
    va_list val;
    va_start(val, fmt);
    npf_vsnprintf(buffer, bufsz, fmt, val);
    va_end(val);
}

static inline void kvsnprintf(char *buffer, size_t bufsz, char const *fmt, va_list args) {
    npf_vsnprintf(buffer, bufsz, fmt, args);
}

#endif // __PRINTF_H__
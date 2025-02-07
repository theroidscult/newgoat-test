#include <printf.h>
#include <panik.h>
static inline void ____kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    kvprintf(fmt, args);
    va_end(args);
}

static inline void ____kcease() {
    panik(ERR_ASSERT_FAILED);
}

#define __kprintf(...) ____kprintf(__VA_ARGS__)
#define __kcease() ____kcease()

#include <KrnlAid/utils/logger.h>
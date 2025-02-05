//nanoprintf
#include <dev/cereal.h>
#include <stdarg.h>

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#define NANOPRINTF_IMPLEMENTATION
#include <KrnlAid/utils/nanoprintf.h>

void print_char(int c, __attribute__((unused)) void *ctx) {
    if(c == '\n') {
        cereal_write(0x3f8, '\r'); //fuck qemu
    }

    cereal_write(0x3f8, (char)c);
}

//string.h
#define STRING_IMPL
#include <string.h>
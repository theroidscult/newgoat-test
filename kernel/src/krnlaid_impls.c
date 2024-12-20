#include <dev/cereal.h>
void print_callback(const char c) {
    cereal_write(0x3F8, c);
}
#define  PRINTF_IMPL
#include <KrnlAid/utils/printf.h>
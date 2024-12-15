
#include <string.h>
#include <dev/cereal.h>

void print_callback(const char c) {
    cereal_write(0x3F8, c);
}
#define  PRINTF_IMPL
#include <KrnlAid/utils/printf.h>

void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

void _start(void) {
    asm("cli");


    cereal_init_port(0x3F8);

    kprintf("Hello, %s! you 0x%p year old!\n", "world", 10000);

    hcf();
}

#include <string.h>

void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

void _start(void) {
    asm("cli");

    hcf();
}
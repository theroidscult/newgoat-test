
#include <string.h>
#include <dev/cereal.h>

void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

void _start(void) {
    asm("cli");

    cereal_init_port(0x3F8);

    cereal_write(0x3F8, 'N');
    cereal_write(0x3F8, 'e');
    cereal_write(0x3F8, 'w');
    cereal_write(0x3F8, 'G');
    cereal_write(0x3F8, 'o');
    cereal_write(0x3F8, 'a');
    cereal_write(0x3F8, 't');
    cereal_write(0x3F8, '!');

    hcf();
}
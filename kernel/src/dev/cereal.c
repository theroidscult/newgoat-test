#include "cereal.h"
#include <KrnlAid/arch/x86_64/portio.h>

int is_transmit_empty(int port) { return inb(port + 5) & 0x20; }
int serial_received(int port) { return inb(port + 5) & 1; }

bool cereal_init_port(int port) {
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x03);
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
    outb(port + 4, 0x1E);
    outb(port + 0, 0xAE);

    if(inb(port + 0) != 0xAE) {
        return false;
    }
    outb(port + 4, 0x0f);
    return true;
}

uint8_t cereal_read(int port_num) {
    __asm__ volatile("cli");
    while(!serial_received(port_num));
    __asm__ volatile("sti");
    return inb(port_num);
}

void cereal_write(int port_num, uint8_t data) {
    __asm__ volatile("cli");
    while(!is_transmit_empty(port_num));
    __asm__ volatile("sti");
    outb(port_num, data);
}
    
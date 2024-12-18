#include "pic.h"

#include <stdint.h>
#include <KrnlAid/arch/x86/portio.h>

void pic_init(void) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();
    outb(PIC1_DATA, 0xFF);
    io_wait();
    outb(PIC2_DATA, 0xFF);
    io_wait();

    outb(PIC1_COMMAND, ICW4_8086);
    io_wait();
    outb(PIC2_COMMAND, ICW4_8086);
    io_wait();
}

void pic_eoi(uint8_t irq) {
    if(irq < 8) {
        outb(PIC1_COMMAND, 0x20);
        io_wait();
    }

    outb(PIC1_COMMAND, 0x20);
    io_wait();
}

void pic_mask(uint8_t irq) {
    uint8_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if(irq > 8) irq -= 8;
    outb(port, inb(port) | (1 << irq));
}

void pic_unmask(uint8_t irq) {
    uint8_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if(irq > 8) irq -= 8;
    outb(port, inb(port) & ~(1 << irq));
}
#include "sys/pit.h"
#include <string.h>
#include <dev/cereal.h>
#include <KrnlAid/arch/x86/gdt.h>
#include <KrnlAid/utils/printf.h>
#include <KrnlAid/arch/x86/gdt.h>

#include <sys/pic.h>
#include <sys/idt.h> 
#include <mm/mm.h>

gdt_entry_t gdt_entries[5] = {{0, 0, 0, 0, 0, 0},
                          {0xFFFF, 0, 0, 0x9A, 0xAF, 0},
                          {0xFFFF, 0, 0, 0x92, 0xCF, 0},
                          {0xFFFF, 0, 0, 0xFA, 0xAF, 0},
                          {0xFFFF, 0, 0, 0xF2, 0xCF, 0}};

void prepare_gdt() {
    gdt_pointer_t pointer = make_gdt_pointer(gdt_entries, 5);
    load_gdt(&pointer);
    flush_cs_ds_etc(0x08, 0x10);
}

extern void prepare_idt();
extern void timer_isr();

void hcf() {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void _start(void) {
    __asm__ volatile("cli");
    cereal_init_port(0x3F8);

    prepare_gdt();
    kprintf("GDT initialized\n");
    prepare_idt();
    pic_init();
    //pit_start(1000);
    //idt_set_irq(0, timer_isr, 0);
    __asm__ volatile ("sti");
    kprintf("Interrupts enabled\n");

    kprintf("Memory initialized\n");
    pmm_init();


    kprintf("Hello World!\n");


    hcf();
}

#include <string.h>
#include <dev/cereal.h>

void print_callback(const char c) {
    cereal_write(0x3F8, c);
}
#define  PRINTF_IMPL
#include <KrnlAid/utils/printf.h>
#include <KrnlAid/arch/x86/gdt.h>

gdt_entry_t gdt_entries[5] = {{0, 0, 0, 0, 0, 0, 0, 0},
                          {0xFFFF, 0, 0, 0x9A, 0xAF, 0, 0, 0},
                          {0xFFFF, 0, 0, 0x92, 0xCF, 0, 0, 0},
                          {0xFFFF, 0, 0, 0xFA, 0xAF, 0, 0, 0},
                          {0xFFFF, 0, 0, 0xF2, 0xCF, 0, 0, 0}};
gdt_pointer_t pointer;

void prepare_gdt() {
    pointer = make_gdt_pointer(gdt_entries, 5);
    load_gdt(&pointer);
    flush_cs_ds_etc(0x08, 0x10);
}

extern void prepare_idt();

void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

void _start(void) {
    asm("cli");
    cereal_init_port(0x3F8);

    prepare_gdt();
    kprintf("GDT initialized\n");
    prepare_idt();
    kprintf("IDT initialized\n");




    kprintf("Hello World!\n");


    hcf();
}
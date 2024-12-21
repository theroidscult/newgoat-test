#include "sys/pit.h"
#include <string.h>
#include <dev/cereal.h>
#include <KrnlAid/arch/x86/gdt.h>
#include <KrnlAid/utils/printf.h>

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
    __asm__ volatile ("cli");
    while(1){
        __asm__ volatile ("hlt");
    }
}

void panik(uint32_t code) {
    switch(code) {
        case 1:
            kprintf("Device not available\n");
            break;
        case 2:
            kprintf("Double fault\n");
            break;
        case 3:
            kprintf("Invalid TSS\n");
            break;
        case 4:
            kprintf("Segment not present\n");
            break;
        case 5:
            kprintf("Stack segment fault\n");
            break;
        case 6:
            kprintf("Machine check\n");
            break;
        case 7:
            kprintf("Outdated or reserved interrupt\n");
    }
    hcf();
}

void _start(void) {
    __asm__ volatile("cli");
    cereal_init_port(0x3F8);

    prepare_gdt();
    kprintf("GDT initialized\n");
    prepare_idt();
    pic_init();
    __asm__ volatile ("sti");
    kprintf("Interrupts enabled\n");
    mm_init();
    kprintf("Memory initialized\n");

    object_t obj = {
        .magic = 0,
        .type = OBJ_TYPE_SCHED_THREAD,
        .data.sched_thread = {
            .id = 0,
            .name_ptr = 1,
            .context = {0}
        }
    };

    kprintf("Object 0's ID: %hhx\n", mm_store_obj(&obj));

    object_t obj2 = {
        .magic = 0,
        .type = OBJ_TYPE_SCHED_PROC_NAME,
        .data.sched_proc_name = {
            .name = "Never gonna give you up"
        }
    };

    kprintf("Object 1's ID: %hhx\n", mm_store_obj(&obj2));

    object_t* test = mm_get_obj(0);

    kprintf("Object 0: magic: %hhx, type: %hhx, name_ptr: %hx, id: %hhx\n", test->magic, test->type, test->data.sched_thread.name_ptr, test->data.sched_thread.id);

    object_t* test2 = mm_get_obj(1);

    kprintf("Object 1: magic: %hhx, type: %hhx, name: \"%s\"\n", test2->magic, test2->type, test2->data.sched_proc_name.name);

    kprintf("Hello World!\n");

    //start the scheduler
    //pit_start(1000);
    //idt_set_irq(0, timer_isr, 0);


    while(1);
}
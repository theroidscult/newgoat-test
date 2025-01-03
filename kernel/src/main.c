#include "limine.h"
#include "sys/pit.h"
#include <stdint.h>
#include <string.h>
#include <dev/cereal.h>
#include <KrnlAid/arch/x86/gdt.h>
#include <printf.h>

#include <sys/pic.h>
#include <sys/idt.h> 
#include <mm/mm.h>
#include <sched/sched.h>

//limine garbage
static volatile struct limine_memmap_request memmap = {
   .id = LIMINE_MEMMAP_REQUEST,
   .revision = 0,
   .response = 0
};

static volatile struct limine_hhdm_request hhdm = {
   .id = LIMINE_HHDM_REQUEST,
   .revision = 0,
   .response = 0
};
static volatile struct limine_module_request modules = {
   .id = LIMINE_MODULE_REQUEST,
   .revision = 0,
   .response = 0
};
struct limine_memmap_response* memmap_ptr = NULL;
struct limine_hhdm_response* hhdm_ptr = NULL;


#define KERNEL_STACK_SIZE 8 * PAGE_SIZE
static uint8_t kernel_stack[KERNEL_STACK_SIZE];

tss_t tss;

struct {
    gdt_entry_t entries[5];
    tss_entry_t tss;
} gdt_entries = {{{0, 0, 0, 0, 0, 0},
                {0xFFFF, 0, 0, 0x9A, 0xAF, 0},
                {0xFFFF, 0, 0, 0x92, 0xCF, 0},
                {0xFFFF, 0, 0, 0xFA, 0xAF, 0},
                {0xFFFF, 0, 0, 0xF2, 0xCF, 0}},
                {0,0,0,0,0,0,0,0}};

void prepare_gdt() {
    gdt_entries.tss = make_tss_entry(&tss);
    gdt_pointer_t pointer = make_gdt_pointer((gdt_entry_t*)&gdt_entries, 7);
    load_gdt(&pointer);
    flush_cs_ds_etc(0x08, 0x10);
    load_tss(0x28);
}

void get_adi_drivers(){
    kprintf("Response revision: %d\n", modules.response->revision);
    kprintf("Response module count: %d\n", modules.response->module_count);
    for(uint64_t i = 0; i < modules.response->module_count; i++){
        if(modules.response->modules[i] == NULL){
            kprintf("Module %d is NULL\n", i);
            continue;
        }
        kprintf("Module address: %p\n", modules.response->modules[i]);
        kprintf("Module: %s\n", modules.response->modules[i]->path);
    }
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
            break;
        case 8:
            kprintf("No process to schedule\n");
            break;
        case 9:
            kprintf("Process is not a process\n");
            break;
    }
    hcf();
}

void testproc(void) {
    kprintf("Welcome to NewGoat!\n");
    while(1);
}

void _start(void) {
    __asm__ volatile("cli");
    cereal_init_port(0x3F8);

    memmap_ptr = (struct limine_memmap_response*)memmap.response;
    hhdm_ptr = (struct limine_hhdm_response*)hhdm.response;

    prepare_gdt();
    kprintf("GDT initialized\n");
    prepare_idt();
    pic_init();
    __asm__ volatile ("sti");
    kprintf("Interrupts enabled\n");
    mm_init();
    kprintf("Memory initialized\n");

    tss.rsp0 = (uint64_t)kernel_stack + KERNEL_STACK_SIZE;
    tss.ist[0] = (uint64_t)kernel_stack + KERNEL_STACK_SIZE;

    get_adi_drivers();

    sched_init();

    sched_new_proc(testproc);

    idt_set_irq(0, timer_isr, 1);
    pic_unmask(0);
    pit_start(1000);

    while(1);
}
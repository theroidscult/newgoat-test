#include "limine.h"
#include "sys/pit.h"
#include <stdint.h>
#include <string.h>

#include <dev/cereal.h>

#include <KrnlAid/arch/x86/gdt.h>
#include <printf.h>
#include "logger.h"

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

extern void prepare_idt();
extern void timer_isr();

void testproc(void) {
    while(1){
        kprintf("a");
    }
}

void testproc2(void) {
    while(1){
        kprintf("b");
    }
}

void _start(void) {
    __asm__ volatile("cli");
    cereal_init_port(0x3F8);

    memmap_ptr = (struct limine_memmap_response*)memmap.response;
    hhdm_ptr = (struct limine_hhdm_response*)hhdm.response;

    prepare_gdt();
    log_debug("GDT initialized\n");
    prepare_idt();
    pic_init();
    __asm__ volatile ("sti");
    log_debug("Interrupts enabled\n");
    mm_init();
    log_debug("Memory initialized\n");

    tss.rsp0 = (uint64_t)kernel_stack + KERNEL_STACK_SIZE;
    tss.ist[0] = (uint64_t)kernel_stack + KERNEL_STACK_SIZE;

    pml_entry_t* pm = pager_create_pml();
    void* stack = HIGHER_HALF(mm_alloc_page());

    memset(stack, 0, PAGE_SIZE);

    pager_map(pm, (uint64_t)stack, (uint64_t)stack, PML_FLAGS_PRESENT | PML_FLAGS_WRITABLE | PML_FLAGS_NO_EXEC);

    object_t proc_obj = {
        .type = OBJ_TYPE_SCHED_THREAD,
        .data = {
            .sched_thread = {
                .name_ptr = 0,
                .pagemap = LOWER_HALF(pm),
                .driver_id = 0,
                .context = {
                    .rax = 0,
                    .rbx = 0,
                    .rcx = 0,
                    .rdx = 0,
                    .rsi = 0,
                    .rdi = 0,
                    .r8 = 0,
                    .r9 = 0,
                    .r10 = 0,
                    .r11 = 0,
                    .r12 = 0,
                    .r13 = 0,
                    .r14 = 0,
                    .r15 = 0,
                    .error = 0,
                    .rip = (uint64_t)testproc,
                    .rsp = (uint64_t)stack + PAGE_SIZE,
                    .rbp = (uint64_t)stack + PAGE_SIZE,
                    .cs = 0x8,
                    .ss = 0x10,
                    .rflags = 0x202
                }
            }
        }
    };

    sched_new_proc(&proc_obj);

    object_t proc_obj2 = {
        .type = OBJ_TYPE_SCHED_THREAD,
        .data = {
            .sched_thread = {
                .name_ptr = 0,
                .pagemap = LOWER_HALF(pm),
                .driver_id = 0,
                .context = {
                    .rax = 0,
                    .rbx = 0,
                    .rcx = 0,
                    .rdx = 0,
                    .rsi = 0,
                    .rdi = 0,
                    .r8 = 0,
                    .r9 = 0,
                    .r10 = 0,
                    .r11 = 0,
                    .r12 = 0,
                    .r13 = 0,
                    .r14 = 0,
                    .r15 = 0,
                    .error = 0,
                    .rip = (uint64_t)testproc2,
                    .rsp = (uint64_t)stack + PAGE_SIZE,
                    .rbp = (uint64_t)stack + PAGE_SIZE,
                    .cs = 0x8,
                    .ss = 0x10,
                    .rflags = 0x202
                }
            }
        }
    };

    sched_new_proc(&proc_obj2);


    mm_obj_print_all();

    idt_set_irq(0, timer_isr, 1);
    pic_unmask(0);
    pit_start(1000);

    while(1);
}
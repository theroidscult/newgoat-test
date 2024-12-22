#include <stdint.h>
#include <sys/pic.h>
#include <dev/cereal.h>
#include <sys/context.h>
#include <mm/mm.h>
#include <string.h>
#include <KrnlAid/utils/printf.h>

extern void panik(uint32_t code);

#define QUEUE_SUZE PAGE_SIZE / 4

uint32_t* proc_queue = 0;
uint32_t  push_index = 0;
uint32_t  pop_index = 0;

uint32_t current_proc_id = 0xFFFFFFFF;

void sched_init() {
    proc_queue = mm_alloc_page();
}

void sys_timer_isr(context_t* context) {
    if(push_index == pop_index) {
        panik(8);
    }

    if(current_proc_id != 0xFFFFFFFF) {
        object_t* current_proc = mm_get_obj(current_proc_id);
        memcpy(&current_proc->data.sched_thread.context, context, sizeof(context_t));
    }

    current_proc_id = proc_queue[pop_index];
    pop_index++;

    if(pop_index > QUEUE_SUZE) {
        pop_index = 0;
    }

    object_t* proc_obj = mm_get_obj(current_proc_id);

    memcpy(context, &proc_obj->data.sched_thread.context, sizeof(context_t));

    if(proc_obj->type != OBJ_TYPE_SCHED_THREAD) {
        panik(9);
    }

    proc_queue[push_index] = current_proc_id;
    push_index++;

    if(push_index > QUEUE_SUZE) {
        push_index = 0;
    }

    pic_eoi(0);
}

void sched_new_proc(void(*proc)(void)) {
    //TODO: VMM 
    void* stack = mm_alloc_page();

    object_t proc_obj = {
        .magic = 0,
        .type = OBJ_TYPE_SCHED_THREAD,
        .data = {
            .sched_thread = {
                .id = push_index,
                .name_ptr = 0,
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
                    .rip = (uint64_t)proc,
                    .rsp = (uint64_t)stack + PAGE_SIZE,
                    .rbp = (uint64_t)stack + PAGE_SIZE,

                    .cs = 0x8,
                    .ss = 0x10,
                    .rflags = 0x202
                }
            }
        }
    };

    uint32_t id = mm_store_obj(&proc_obj);
    proc_queue[push_index] = id;
    push_index++;

    if(push_index > QUEUE_SUZE) {
        push_index = 0;
    }
}
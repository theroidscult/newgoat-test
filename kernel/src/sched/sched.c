#include "mm/pager.h"
#include <stdint.h>
#include <sys/pic.h>
#include <dev/cereal.h>
#include <sys/context.h>
#include <mm/mm.h>
#include <string.h>
#include <printf.h>

extern void panik(uint32_t code);

#define QUEUE_SUZE PAGE_SIZE / sizeof(uint32_t)

uint32_t proc_queue[QUEUE_SUZE];

uint32_t  push_index = 0;
uint32_t  pop_index = 0;

uint32_t current_proc_id = 0xFFFFFFFF;

void sys_timer_isr(context_t* context) {
    mm_restore_kernel_pm();

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

    pager_set_current_pml(proc_obj->data.sched_thread.pagemap);
    pic_eoi(0);
}

void sched_new_proc(object_t* proc_obj) {
    uint32_t id = mm_store_obj(proc_obj);
    if(push_index > QUEUE_SUZE) {
        push_index = 0;
    }
    proc_queue[push_index] = id;
    push_index++;

}

uint32_t sched_get_next_proc_id() {
    return push_index;
}
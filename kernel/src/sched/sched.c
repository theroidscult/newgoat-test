#include "logger.h"
#include "mm/pager.h"
#include <stdint.h>
#include <sys/pic.h>
#include <dev/cereal.h>
#include <sys/context.h>
#include <mm/mm.h>
#include <string.h>
#include <printf.h>
#include <panik.h>

extern void panik(uint32_t code);

#define QUEUE_SUZE PAGE_SIZE / sizeof(uint32_t)

uint32_t proc_queue[QUEUE_SUZE];

uint32_t  push_index = 0;
object_t* current_proc = (void*)NULL;

uint32_t current_proc_id = 0xFFFFFFFF;

void sys_timer_isr(context_t* context) {
    mm_restore_kernel_pm();

    if(push_index == 0) {
        panik(ERR_NO_PROCESS_TO_SCHEDULE);
    }

    if(current_proc_id != 0xFFFFFFFF) {
        memcpy(&current_proc->data.sched_thread.context, context, sizeof(context_t));
        
        log_info("Putting back process with id %d at %d\n", current_proc_id, push_index-1);
        proc_queue[push_index-1] = current_proc_id;
    }
    

    log_info("Switching to process with id %d\n", proc_queue[0]);
    current_proc_id = proc_queue[0];
    memcpy(proc_queue, proc_queue + 1, (push_index - 1) * sizeof(uint32_t));

    current_proc = mm_get_obj(current_proc_id);
    memcpy(context, &current_proc->data.sched_thread.context, sizeof(context_t));

    if(current_proc->type != OBJ_TYPE_SCHED_THREAD) {
        panik(ERR_PROCESS_IS_NOT_A_PROCESS);
    }

    pager_set_current_pml(current_proc->data.sched_thread.pagemap);
    pic_eoi(0);
}

uint32_t sched_get_current_proc_id() {
    return current_proc_id;
}

uint32_t sched_new_proc(object_t* proc_obj) {
    if(push_index + 1 > QUEUE_SUZE) {
        return 0;
    }
    uint32_t id = mm_store_obj(proc_obj);
    proc_queue[push_index] = id;
    push_index++;

    log_info("New process created with id %d, process queue index %d\n", id, push_index - 1);

    return id;
}
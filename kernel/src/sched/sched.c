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

uint32_t current_proc_id = 0xFFFFFFFF;
object_t* current_proc = (void*)NULL;

//TODO: encoorporate this into a per-CPU object
uint32_t queue[QUEUE_SUZE] = {0};
uint32_t queue_size = 0;
uint32_t queue_head = 0;
uint32_t queue_tail = 0;

bool enqueue(uint32_t proc_id) {
    if (queue_size == QUEUE_SUZE) {
        return false;
    }

    queue[queue_tail] = proc_id;
    queue_tail = (queue_tail + 1) % QUEUE_SUZE;
    queue_size++;
    return true;
}

bool dequeue(uint32_t* proc_id) {
    if (queue_size == 0) {
        return false;
    }

    *proc_id = queue[queue_head];
    queue_head = (queue_head + 1) % QUEUE_SUZE;
    queue_size--;
    return true;
}


void sys_timer_isr(context_t* context) {
    mm_restore_kernel_pm();

    if (current_proc_id != 0xFFFFFFFF) {
        current_proc->data.sched_thread.context = *context;
        if(!enqueue(current_proc_id)) {
            log_error("Failed to enqueue process with id %d\n", current_proc_id);
        }
    }

    if (!dequeue(&current_proc_id)) {
        panik(ERR_NO_PROCESS_TO_SCHEDULE);
    }

    log_info("Switching to process with id %d\n", current_proc_id);

    current_proc = mm_get_obj(current_proc_id);

    if (current_proc->type != OBJ_TYPE_SCHED_THREAD) {
        log_error("Current proc has invalid type\n");
        panik(ERR_PROCESS_IS_NOT_A_PROCESS);
    }

    *context = current_proc->data.sched_thread.context;

    pic_eoi(0);
}

uint32_t sched_get_current_proc_id() {
    return current_proc_id;
}

uint32_t sched_new_proc(object_t* proc_obj) {
    uint32_t id = mm_store_obj(proc_obj);

    if (!enqueue(id)){
        log_error("Failed to enqueue process with id %d\n", id);
        return 0;
    }

    log_info("New process created with id %d,\n", id);

    return id;
}
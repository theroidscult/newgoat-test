#ifndef __SCHED_H__
#define __SCHED_H__

#include <stdint.h>
#include <mm/mm.h>

void sched_new_proc(object_t* proc_obj); 
uint32_t sched_get_next_proc_id(void);

#endif // __SCHED_H__
#ifndef __SCHED_H__
#define __SCHED_H__

#include <stdint.h>
#include <mm/mm.h>

uint32_t sched_new_proc(object_t* proc_obj); 

#endif // __SCHED_H__
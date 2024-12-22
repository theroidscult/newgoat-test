#ifndef __SCHED_H__
#define __SCHED_H__

#include <stdint.h>

void sched_init(void);
void sched_new_proc(void(*proc)(void)); 

#endif // __SCHED_H__
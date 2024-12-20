#ifndef __PMM_H__
#define __PMM_H__

#include <stdint.h>
#include <limine_garbage.h>

typedef struct __freelist_entry {
    struct __freelist_entry *next;
    uint64_t size;
} pmm_freelist_entry_t;

void pmm_init(void);

#endif // __PMM_H__
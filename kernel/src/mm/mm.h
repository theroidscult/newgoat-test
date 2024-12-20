#ifndef __PMM_H__
#define __PMM_H__

#include <stdint.h>
#include <limine_garbage.h>

#define PAGE_SIZE 0x1000

#define HIGHER_HALF(addr) ((addr) + 0xFFFF800000000000)
#define LOWER_HALF(addr) ((addr) - 0xFFFF800000000000)

typedef struct __freelist_entry {
    struct __freelist_entry *next;
    uint64_t size;
} freelist_entry_t;

void mm_init(void);

#endif // __PMM_H__
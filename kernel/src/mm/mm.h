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

typedef struct {
    uint64_t total;
    uint64_t usable;
    uint64_t used;
    uint64_t free;
} memstats_t;

void mm_init(void);
memstats_t mm_poll_mstats(void);
void* mm_alloc_page();
void mm_free_pages(void* page, uint64_t size);

#endif // __PMM_H__
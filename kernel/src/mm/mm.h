#ifndef __PMM_H__
#define __PMM_H__

#include <stdint.h>
#include <sys/context.h>
#include "limine.h"
#include "pager.h"

#define PAGE_SIZE 0x1000

extern uint64_t hhdm_offset;

#define HIGHER_HALF(addr) (void*)(((uint64_t)addr) + hhdm_offset)
#define LOWER_HALF(addr) (void*)(((uint64_t)addr) - hhdm_offset)

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

#define OBJ_MAGIC 0x55
#define OBJ_MAGIC_FREE 0

#define OBJ_TYPE_SCHED_THREAD 0
#define OBJ_TYPE_SCHED_PROC_NAME 1

typedef struct {
    uint8_t magic;
    uint8_t type;
    union {
        struct {
            uint16_t id;
            uint32_t name_ptr;
            pml_entry_t* pagemap;
            context_t context;
            char slack[64];
        } __attribute__((packed)) sched_thread;
        struct {
            uint32_t next_page;
            char name[250];
        } __attribute__((packed)) sched_proc_name;
    } data;
} object_t;

void mm_init(void);
memstats_t mm_poll_mstats(void);
void* mm_alloc_page();
void mm_free_pages(void* page, uint64_t size);

void mm_restore_kernel_pm();

uint32_t mm_store_obj(object_t* obj);
object_t* mm_get_obj(uint32_t id);
void mm_free_obj(uint32_t id);

#endif // __PMM_H__
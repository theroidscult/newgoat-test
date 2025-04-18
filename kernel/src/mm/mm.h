#ifndef __PMM_H__
#define __PMM_H__

#include <stdint.h>
#include <sys/context.h>
#include "kernel.h"
#include "pager.h"

#define PAGE_SIZE 0x1000

#define HIGHER_HALF(addr) (void*)(((uint64_t)addr) + hhdm_ptr->offset)
#define LOWER_HALF(addr) (void*)(((uint64_t)addr) - hhdm_ptr->offset)

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

#define OBJ_TYPE_NONE 0
#define OBJ_TYPE_GENERIC_NAME 1
#define OBJ_TYPE_GENERIC_FPTRS 2
#define OBJ_TYPE_SCHED_THREAD 3

typedef uint32_t object_id_t;

typedef struct {
    uint16_t type;
    union {
        struct {
            object_id_t next_page;
            char name[250];
        } __attribute__((packed)) generic_name;
        struct {
            object_id_t next_page;
            uintptr_t fptrs[31];
        } __attribute__((packed)) generic_fptrs;
        struct {
            object_id_t name_ptr; 
            pml_entry_t* pagemap;
            context_t context;
            object_id_t driver_id;
            char slack[70];
        } __attribute__((packed)) sched_thread;
    } data;
} object_t;

void mm_init(void);
memstats_t mm_poll_mstats(void);
void* mm_alloc_pages(uint64_t count);
void mm_free_pages(void* page, uint64_t size);

void mm_restore_kernel_pm();

uint32_t mm_store_obj(object_t* obj);
object_t* mm_get_obj(uint32_t id);
void mm_free_obj(uint32_t id);

void mm_obj_print_all();

#endif // __PMM_H__
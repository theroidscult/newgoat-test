#include "mm.h"
#include "limine.h"
#include "mm/pager.h"
#include <stdint.h>
#include <printf.h>
#include <string.h>
#include <kernel.h>

freelist_entry_t *head = NULL;
freelist_entry_t *tail = NULL;


//kernel stuff
pml_entry_t* kernel_pm = NULL;

// object space
void* object_space = NULL;
const uint64_t object_space_size = 10; // IN PAGES
void* object_first_free = NULL;


//stats
uint64_t total_mem_size = 0;
uint64_t usable_mem_size = 0;
uint64_t free_mem_size = 0;
uint64_t used_mem_size = 0;

void mm_init(void) {
    for (uint64_t i = 0; i < memmap_ptr->entry_count; i++) {
        if(memmap_ptr->entries[i]->type == LIMINE_MEMMAP_USABLE) {

            freelist_entry_t *newhead = (freelist_entry_t *) HIGHER_HALF(memmap_ptr->entries[i]->base);
            newhead->size = memmap_ptr->entries[i]->length / PAGE_SIZE;

            if(head == NULL) {
                head = newhead;
                tail = newhead;
                continue;
            }

            head->next = newhead;
            head = newhead;

            usable_mem_size += memmap_ptr->entries[i]->length;
        }
        total_mem_size += memmap_ptr->entries[i]->length;
    }
    head->next = NULL;

    free_mem_size = usable_mem_size;

    //reserve object space
    object_space = (void*)mm_alloc_page();
    object_first_free = object_space;
    for(uint64_t i = 0; i < object_space_size - 1; i++) {
        mm_alloc_page(); //allocate the next page too!
    }

    //set up the kernel PM
    kernel_pm = pager_get_current_pml();

    kprintf("Total memory: %u B\n", total_mem_size);
    kprintf("Usable memory: %u B\n", usable_mem_size);
    kprintf("Free memory: %u B\n", free_mem_size);
    kprintf("Used memory: %u B\n", used_mem_size);
}

memstats_t mm_poll_mstats(void) {
    return (memstats_t){
        .total = total_mem_size,
        .usable = usable_mem_size,
        .free = free_mem_size,
        .used = used_mem_size
    };
}

void* mm_alloc_page() {
    freelist_entry_t *toret = LOWER_HALF(tail);
    if(tail == NULL) {
        return NULL;
    }
    if(tail->size > 1) {
        freelist_entry_t *new_tail = (freelist_entry_t*)((char*)tail + PAGE_SIZE);
        new_tail->size = tail->size - 1;
        new_tail->next = tail->next;
        tail = new_tail;
    } else {
        tail = tail->next;
    }
    free_mem_size -= PAGE_SIZE;
    used_mem_size += PAGE_SIZE;
    return (void*)toret;
}

void mm_free_pages(void* page, uint64_t size) {
    if((uintptr_t)page < (uintptr_t)tail) {
        if((uintptr_t)page + (size * PAGE_SIZE) > (uintptr_t)tail) {
            return;
        }
        freelist_entry_t *newtail = (freelist_entry_t*)page;
        newtail->next = tail;
        newtail->size = size;
        tail = newtail;
    } else {
        freelist_entry_t *cur = tail;
        while(cur != NULL) {
            if((uintptr_t)cur->next > (uintptr_t)page) {
                freelist_entry_t *newent = (freelist_entry_t*)page;
                newent->next = cur->next;
                newent->size = size;
                cur->next = newent;
                return;
            }
            cur = cur->next;
        }
    }
    free_mem_size += size * PAGE_SIZE;
    used_mem_size -= size * PAGE_SIZE;
}

void mm_restore_kernel_pm() {
    pager_set_current_pml(kernel_pm);
}

uint32_t mm_store_obj(object_t* obj)
{
    if(object_first_free == NULL) {
        return 0; // out of space; TODO: add more space
    }
    uint32_t id = ((uintptr_t)object_first_free - (uintptr_t)object_space) / sizeof(object_t);
    memcpy(object_first_free, obj, sizeof(object_t));
    object_t* cur = object_first_free; 
    while((uintptr_t)cur < (uintptr_t)object_space + (object_space_size * PAGE_SIZE)) {
        if(cur->type == OBJ_TYPE_NONE) {
            object_first_free = cur;
            return id;
        }
        cur++;
    }
    object_first_free = NULL;
    return id;
}

object_t* mm_get_obj(uint32_t id)
{
    return object_space + (id * sizeof(object_t));
}

void mm_free_obj(uint32_t id)
{
    object_t* obj = object_space + (id * sizeof(object_t));
    obj->type = OBJ_TYPE_NONE;
    if((uintptr_t)object_first_free > (uintptr_t)obj) {
        object_first_free = (void*)obj;
    }
}
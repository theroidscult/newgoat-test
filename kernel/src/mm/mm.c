#include "mm.h"
#include "limine.h"
#include <stdint.h>
#include <KrnlAid/utils/printf.h>

freelist_entry_t *head = NULL;
freelist_entry_t *tail = NULL;

void* object_space = NULL;
uint64_t object_space_size = 0;

uint64_t usable_mem_size = 0;

void mm_init(void) {
    struct limine_memmap_response *response = memmap.response;

    for (uint64_t i = 0; i < response->entry_count; i++) {
        if(response->entries[i]->type == LIMINE_MEMMAP_USABLE ||
           response->entries[i]->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE ||
           response->entries[i]->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {

            freelist_entry_t *newhead = (freelist_entry_t *) HIGHER_HALF(response->entries[i]->base);
            newhead->size = response->entries[i]->length / PAGE_SIZE;

            if(head == NULL) {
                head = newhead;
                tail = newhead;
                continue;
            }

            head->next = newhead;
            head = newhead;

            usable_mem_size += response->entries[i]->length;
        }
    }
    head->next = NULL;

    kprintf("Usable memory(before object space allocation): %u B\n", usable_mem_size);

    //reserve object space
    freelist_entry_t *prev = NULL;
    freelist_entry_t *next = NULL;
    freelist_entry_t *ent = head;
    
    while(ent) {
        if(ent->size > object_space_size) {
            object_space = (void*)ent;
            object_space_size = ent->size;
            prev = ent;
            if(ent->next != NULL) {
                next = ent->next->next;
            }
            continue;
        }
        ent = ent->next;
    }

    if(object_space == NULL) {
        kprintf("failed to reserve object space\n");
        return;
    }

    //remove object space from free list
    if(next != NULL) {
        prev->next = next;
    }
    kprintf("Memory %% used by object space: %u%%\n", (object_space_size * PAGE_SIZE * 100) / usable_mem_size);
    usable_mem_size -= object_space_size * PAGE_SIZE;

    kprintf("Usable memory(after object space allocation):  %u B\n", usable_mem_size);
}

void* alloc_page() {
    freelist_entry_t *toret = tail;
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
    usable_mem_size += PAGE_SIZE;
    return (void*)((char*)toret + PAGE_SIZE);
}
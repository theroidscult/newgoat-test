#include "mm.h"
#include "limine.h"
#include <stdint.h>
#include <KrnlAid/utils/printf.h>

freelist_entry_t *head = NULL;
freelist_entry_t *tail = NULL;

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
                return;
            }

            head->next = newhead;
            head = newhead;

        }
    }
}
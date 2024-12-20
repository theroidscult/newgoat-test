#include "pmm.h"
#include <stdint.h>
#include <KrnlAid/utils/printf.h>

void pmm_init(void) {
    struct limine_memmap_response *response = memmap.response;

    for (uint64_t i = 0; i < response->entry_count; i++) {
        kprintf("Found memory at 0x%p with size 0x%p of type 0x%u\n", response->entries[i]->base, response->entries[i]->length,response->entries[i]->type);
    }
}
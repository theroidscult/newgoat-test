#include "pager.h"
#include "mm.h"
#include <stddef.h>
#include <stdint.h>

pml_entry_t* pager_get_current_pml(void){
    pml_entry_t* toret = NULL;

    __asm__ volatile ("mov %%cr3, %0" : "=r" (toret));

    return toret;
}

void pager_set_current_pml(pml_entry_t* pml){
    __asm__ volatile ("mov %0, %%cr3" : : "r" (pml));
}

void pager_invlpg(uint64_t va){
    __asm__ volatile ("invlpg (%0)" : : "r" (va));
}


pml_entry_t* pager_create_pml(void){
    pml_entry_t* toret = mm_alloc_page();
    pml_entry_t* cur = pager_get_current_pml();

    for(int i = 0; i < 512; i++){
        if(i < 256)
            toret[i] = 0;
        else
         toret[i] = cur[i];
    }

    return toret;
}

pml_entry_t* pager_get_next_lvl(pml_entry_t* prev, uint64_t index,uint64_t flags){
    pml_entry_t entry = prev[index];
    if(!(entry & PML_FLAGS_PRESENT)){
        void* pa = mm_alloc_page();
        prev[index] = (uint64_t)pa | flags;
        return (pml_entry_t*)PML_GET_ADDR((uint64_t)pa);
    }

    return (pml_entry_t*)PML_GET_ADDR(entry);
}

void pager_map(pml_entry_t* pml, uint64_t va, uint64_t pa, uint64_t flags){
    uint64_t pml4_index = (va >> 39) & 0x1ff;
    uint64_t pml3_index = (va >> 30) & 0x1ff;
    uint64_t pml2_index = (va >> 21) & 0x1ff;

    pml_entry_t* pml3 = pager_get_next_lvl(pml, pml4_index, flags);
    pml_entry_t* pml2 = pager_get_next_lvl(pml3, pml3_index, flags);
    pml_entry_t* page = pager_get_next_lvl(pml2, pml2_index, flags);

    *page = pa | flags;
}

void pager_unmap(pml_entry_t* pml, uint64_t va){
        uint64_t pml4_index = (va >> 39) & 0x1ff;
    uint64_t pml3_index = (va >> 30) & 0x1ff;
    uint64_t pml2_index = (va >> 21) & 0x1ff;

    pml_entry_t* pml3 = pager_get_next_lvl(pml, pml4_index, 0);
    pml_entry_t* pml2 = pager_get_next_lvl(pml3, pml3_index, 0);
    pml_entry_t* page = pager_get_next_lvl(pml2, pml2_index, 0);

    *page = 0;
}
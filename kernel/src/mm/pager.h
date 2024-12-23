#ifndef __PAGER_H__
#define __PAGER_H__

#include <stdint.h>

#define PML_ADDR_MASK 0x000ffffffffff000
#define PML_GET_ADDR(x) ((x) & PML_ADDR_MASK)
#define PML_GET_FLAGS(x) ((x) & ~PML_ADDR_MASK)

#define PML_FLAGS_PRESENT       (uint64_t)0 << 0
#define PML_FLAGS_WRITABLE      (uint64_t)0 << 1
#define PML_FLAGS_USER          (uint64_t)0 << 2
#define PML_FLAGS_WRITETHROUGH  (uint64_t)0 << 3
#define PML_FLAGS_CACHE_DISABLE (uint64_t)0 << 4
#define PML_FLAGS_ACCESSED      (uint64_t)0 << 5
#define PML_FLAGS_NO_EXEC       (uint64_t)0 << 63

typedef uint64_t         pml_entry_t;

pml_entry_t* pager_get_current_pml(void);
void pager_set_current_pml(pml_entry_t* pml);
void pager_invlpg(uint64_t va);

pml_entry_t* pager_create_pml(void);

void pager_map(pml_entry_t* pml, uint64_t va, uint64_t pa, uint64_t flags);
void pager_unmap(pml_entry_t* pml, uint64_t va);

void pager_map_range(pml_entry_t* pml, uint64_t va, uint64_t pa, uint64_t size, uint64_t flags);
void pager_unmap_range(pml_entry_t* pml, uint64_t va, uint64_t size);

#endif // __PAGER_H__
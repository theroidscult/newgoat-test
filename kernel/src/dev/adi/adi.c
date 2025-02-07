#include "adi.h"
#include "adi_ff.h"
#include "adi_libs.h"

#include "mm/mm.h"
#include <printf.h>
#include <mm/pager.h>
#include <sched/sched.h>
#include <stdint.h>
#include <string.h>

#include <KrnlAid//utils/math.h>

uint32_t adi_load(const char* drv_file,__attribute__((unused)) uint32_t size){
    adi_ff_header_t* header = (adi_ff_header_t*)drv_file;
    char* string_table = (char*)drv_file + header->string_table_offset;
    adi_ff_metalang_t* metalang_table = (adi_ff_metalang_t*)(drv_file + header->metalang_table_offset);
    adi_ff_segment_t* segment_table = (adi_ff_segment_t*)(drv_file + header->segment_table_offset);

    if(header->magic_num != ADI_FF_MAGIC) {
        kprintf("[ADI Error] Magic number mismatch\n");
        goto error;
    }

    if(header->spec_version != 1) {
        kprintf("[ADI Error] Unsupported spec version %d\n", header->spec_version);
        goto error;
    }

    void* pagemap = pager_create_pml();

    for(uint64_t i = 0; i < header->metalang_table_size / sizeof(adi_ff_metalang_t); i++){
        switch (metalang_table[i].id) {
            case 0x00000001:
                kprintf("[ADI] Driver uses the x86_64 Metalanguage\n");
                break;
            case 0x00010000:
                kprintf("[ADI] Driver uses the PCI Metalanguage\n");
                break;
            case 0x400F0000:
                kprintf("[ADI] Driver uses the Storage Metalanguage\n");
                break;
            default:
                kprintf("[ADI] Driver uses an unsupported Metalanguage (0x%08x)\n", metalang_table[i].id);
                goto error;
        }
    }

    for(uint64_t i = 0; i < header->segment_table_size / sizeof(adi_ff_segment_t); i++){
        adi_ff_segment_t* segment = &segment_table[i];

        uint64_t flags = (segment->flags & ADI_FF_SEGMENT_FLAG_EXEC) ? 0 : PML_FLAGS_NO_EXEC;
        if(segment->flags & ADI_FF_SEGMENT_FLAG_READ) flags |= PML_FLAGS_PRESENT;
        if(segment->flags & ADI_FF_SEGMENT_FLAG_WRITE) flags |= PML_FLAGS_WRITABLE;


        pager_map_range(pagemap, ALIGN_DOWN(segment->virt_addr, PAGE_SIZE), (uint64_t)ALIGN_DOWN(LOWER_HALF((uint64_t)drv_file + segment->segment_offset),PAGE_SIZE), DIV_ROUND_UP(segment->segment_size,PAGE_SIZE), flags);

        kprintf("[ADI] Segment \"%s\" mapped at 0x%lx spaning 0x%x pages with falgs 0x%lx!\n", string_table + segment->name_offset, segment->virt_addr, DIV_ROUND_UP(segment->segment_size,PAGE_SIZE), flags);
    }

    //TODO: userspace drivers
    void* stack = mm_alloc_page();
    memset(HIGHER_HALF(stack), 0, PAGE_SIZE);
    pager_map(pagemap, (uint64_t)stack, (uint64_t)stack, PML_FLAGS_PRESENT | PML_FLAGS_WRITABLE | PML_FLAGS_NO_EXEC);

    object_t driver_name_obj = {
        .type = OBJ_TYPE_GENERIC_NAME,
        .data = {
            .generic_name = {
                .next_page = 0,
                .name = ""
            }
        }
    };

    strncpy(driver_name_obj.data.generic_name.name, string_table + header->name_offset, 250);

    uint32_t name_id = mm_store_obj(&driver_name_obj);

    object_t driver_author_obj = {
        .type = OBJ_TYPE_GENERIC_NAME,
        .data = {
            .generic_name = {
                .next_page = 0,
                .name = ""
            }
        }
    };

    strncpy(driver_author_obj.data.generic_name.name, string_table + header->author_offset, 250);

    uint32_t author_id = mm_store_obj(&driver_author_obj);

    object_t driver_obj = {
        .type = OBJ_TYPE_DRIVER,
        .data = {
            .driver = {
                .name_ptr = name_id,
                .author_ptr = author_id,
                .ver_major = header->ver_major,
                .ver_minor = header->ver_minor,
                .ver_patch = header->ver_patch,
                .devices = 0,
            }
        }
    };

    uint32_t driver_id = mm_store_obj(&driver_obj);

    object_t proc_obj = {
        .type = OBJ_TYPE_SCHED_THREAD,
        .data = {
            .sched_thread = {
                .name_ptr = 0,
                .pagemap = LOWER_HALF(pagemap),
                .driver_id = driver_id,
                .context = {
                    .rax = 0,
                    .rbx = 0,
                    .rcx = 0,
                    .rdx = 0,
                    .rsi = 0,
                    .rdi = (uint64_t)&core,
                    .r8 = 0,
                    .r9 = 0,
                    .r10 = 0,
                    .r11 = 0,
                    .r12 = 0,
                    .r13 = 0,
                    .r14 = 0,
                    .r15 = 0,
                    .error = 0,
                    .rip = (uint64_t)header->entry_point,
                    .rsp = (uint64_t)stack + PAGE_SIZE,
                    .rbp = (uint64_t)stack + PAGE_SIZE,
                    .cs = 0x8,
                    .ss = 0x10,
                    .rflags = 0x202
                }
            }
        }
    };

    sched_new_proc(&proc_obj); 

    kprintf("[ADI] Loaded driver \"%s\" by \"%s\"\n",  string_table + header->name_offset, string_table + header->author_offset);

    memcpy((char*)drv_file, drv_file + header->content_region_offset, size - header->content_region_offset); // took me so long tofigure this out

    return 1;

    error:
    kprintf("[ADI] Error loading driver\n");
    return 0;
}
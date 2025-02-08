#include "adi_libs.h"
#include "libadi/types.h"
#include <printf.h>
#include <stdint.h>
#include <string.h>
#include <logger.h>

#include <mm/mm.h>

metalang_x86_64_t x86_64_m = (metalang_x86_64_t){
    
};

metalang_pci_t bus_pci_m = (metalang_pci_t){
    
};

metalang_kb_t kb_m = (metalang_kb_t){
    
};

metalang_pointer_t pointer_m = (metalang_pointer_t){
    
};

metalang_screenmgmt_t screenmgmt_m = (metalang_screenmgmt_t){
    
};

metalang_fb_t fb_m = (metalang_fb_t){
    
};

metalanguage_t storage_init(__attribute__((unused)) event_identify identify,__attribute__((unused)) event_transact transact){
    log_trace("storage_init\n");

    if(identify == NULL || transact == NULL) {
        log_trace("Storage metalang implementation failed to register\n");
        return (metalanguage_t){.id = 0, .name = NULL, .lang_impl = 0, .params = NULL};
    }

    object_t fptrs = (object_t){
        .type = OBJ_TYPE_GENERIC_FPTRS,
        .data.generic_fptrs = {
            .fptrs[0] = (uintptr_t)identify,
            .fptrs[1] = (uintptr_t)transact
        }
    };

    uint32_t fptrs_id = mm_store_obj(&fptrs);

    object_t timekeeper = (object_t){
        .type = OBJ_TYPE_METALANG_IMPL,
        .data.metalang_impl = {
            .id    = 0x400F0000, //TODO: make it a define
            .fptrs = fptrs_id
        }
    };

    uint32_t metalang_id = mm_store_obj(&timekeeper);

    log_trace("Storage metalang implementation registered");

    return (metalanguage_t){.id = 0x400F0000, .name = "misc_storage", .lang_impl = (uintptr_t)metalang_id, .params = NULL};
}

bool storage_signal_transaction_done(__attribute__((unused)) adi_device_t* dev,__attribute__((unused)) uint32_t id){
    kprintf("[STUB] storage_signal_transaction_done\n");
    return true;
}

metalang_storage_t storage_m = (metalang_storage_t){
    .init = storage_init,
    .signal_transaction_done = NULL
};

metalanguage_t timekeeper_init(__attribute__((unused)) event_get_time get_time,__attribute__((unused)) event_set_time set_time,__attribute__((unused)) event_get_time_unix get_time_unix,__attribute__((unused)) event_set_time_unix set_time_unix){
    log_trace("timekeeper_init\n");

    if(get_time == NULL || set_time == NULL || get_time_unix == NULL || set_time_unix == NULL) {
        log_debug("Timekeeper initialization failed\n");
        return (metalanguage_t){.id = 0, .name = NULL, .lang_impl = 0, .params = NULL};
    }

    object_t fptrs = (object_t){
        .type = OBJ_TYPE_GENERIC_FPTRS,
        .data.generic_fptrs = {
            .fptrs[0] = (uintptr_t)get_time,
            .fptrs[1] = (uintptr_t)set_time,
            .fptrs[2] = (uintptr_t)get_time_unix,
            .fptrs[3] = (uintptr_t)set_time_unix
        }
    };

    uint32_t fptrs_id = mm_store_obj(&fptrs);

    object_t timekeeper = (object_t){
        .type = OBJ_TYPE_METALANG_IMPL,
        .data.metalang_impl = {
            .id    = 0x400F0001, //TODO: make it a define
            .fptrs = fptrs_id
        }
    };

    uint32_t metalang_id = mm_store_obj(&timekeeper);

    return (metalanguage_t){.id = 0x400F0001, .name = "misc_timekeeper", .lang_impl = (uintptr_t)metalang_id, .params = NULL};
}

metalang_timekeeper_t timekeeper_m = (metalang_timekeeper_t){
    .init = timekeeper_init
};

void _log_info(char* format,...) {
    char buffer[1024];
    ksnprintf(buffer, 1024,"[LOG] %s", format);
    va_list args;
    va_start(args,format); 
    kvprintf(buffer,args);
    va_end(args);
}
void _log_warning(char* format,...) { 
    char buffer[1024];
    ksnprintf(buffer, 1024,"[WARN] %s", format);
    va_list args;
    va_start(args,format); 
    kvprintf(buffer,args);
    va_end(args);
}
void _log_error(char* format,...) { 
    char buffer[1024];
    ksnprintf(buffer, 1024,"[ERROR] %s", format);
    va_list args;
    va_start(args,format); 
    kvprintf(buffer,args);
    va_end(args);
}

void _panic(char* format,...) {
    char buffer[1024];
    ksnprintf(buffer, 1024,"[PANIC] %s", format);
    va_list args;
    va_start(args,format); 
    kvprintf(buffer,args);
    va_end(args);
    while(1);
}

void exit(bool success) {
    kprintf("[ADI] Driver Initialization %s\n" , success ? "succeeded" : "failed");
    while(1);
}

uint32_t register_device(__attribute__((unused)) metalanguage_t* metalangs,uint32_t count) {
    kprintf("[STUB] Registering device with %d metalanguages\n", count);
    return 1;
}

void unregister_device(uint32_t id) {
    kprintf("[STUB] Unregistering device with id %d\n", id);
}

void *malloc(__attribute__((unused)) size_t size) {
    return (void*)0;
}

bool free(__attribute__((unused)) void *ptr) {
    return true;
}

void *realloc(__attribute__((unused)) void *ptr,__attribute__((unused)) size_t size) {
    return (void*)0;
}

bool alib_memcpy(void *dest, void *src, size_t n) {
    memcpy(dest, src, n);
    return true;
}

bool alib_memset(void *s, uint8_t c, size_t n) {
    memset(s, c, n);
    return true;
}

adi_core_t core = (adi_core_t){
    .arch_x86_64 = &x86_64_m,
    .bus_pci = &bus_pci_m,
    .hid_pointer = &pointer_m,
    .hid_kb = &kb_m,
    .video_screenmgmt = &screenmgmt_m,
    .video_fb = &fb_m,
    .misc_storage = &storage_m,
    .misc_timekeeper = &timekeeper_m,

    .log_info = _log_info,
    .log_warning = _log_warning,
    .log_error = _log_error,
    .panic = _panic,
    .exit = exit,

    .register_device = register_device,
    .unregister_device = unregister_device,

    .alloc = malloc,
    .free = free,
    .realloc = realloc,

    .memcpy = alib_memcpy,
    .memset = alib_memset,
};
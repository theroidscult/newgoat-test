#include "panik.h"
#include <stdint.h>
#include <printf.h>

void panik(uint32_t code) {
    switch(code) {
        //Fatal exceptions
        case ERR_DEVICE_NOT_AVAILABLE:
            kprintf("Device not available\n");
            break;
        case ERR_DOUBLE_FAULT:
            kprintf("Double fault\n");
            break;
        case ERR_INVALID_TSS:
            kprintf("Invalid TSS\n");
            break;
        case ERR_SEGMENT_NOT_PRESENT:
            kprintf("Segment not present\n");
            break;
        case ERR_STACK_SEGMENT_FAULT:
            kprintf("Stack segment fault\n");
            break;
        case ERR_MACHINE_CHECK:
            kprintf("Machine check\n");
            break;
        case ERR_OUTDATED_OR_RESERVED_INT:
            kprintf("Outdated or reserved interrupt\n");
            break;

        //Process errors
        case ERR_NO_PROCESS_TO_SCHEDULE:
            kprintf("No process to schedule\n");
            break;
        case ERR_PROCESS_IS_NOT_A_PROCESS:
            kprintf("Process is not a process\n");
            break;
        case ERR_PROCESS0_KILLED:
            kprintf("Process0 killed\n");
            break;

        //Memory errors
        case ERR_OUT_OF_MEMORY:
            kprintf("Out of memory\n");
            break;

        //Registery errors
        case ERR_NO_REGISTERY_FILE:
            kprintf("No registery file\n");
            break; 
        case ERR_CORRUPTED_REGISTERY:
            kprintf("Corrupted registery\n");
            break;


        //INIT errors
        case ERR_NO_ROOT_FILESYSTEM:
            kprintf("No root filesystem\n");
            break;
        case ERR_NO_INIT_KEY_IN_REGISTERY:
            kprintf("No init key in registery\n");
            break;
        case ERR_REQUIRED_INIT_STEP_FAILED:
            kprintf("Required init step failed\n");
            break;


        //Misc errors
        case ERR_ASSERT_FAILED:
            kprintf("Assert failed\n");
            break;

        default: 
            kprintf("Unknown error code %d\n", code);
            break;
    }

    //TODO: register dumping

    __asm__ volatile ("cli");
    while(1){
        __asm__ volatile ("hlt");
    }
}
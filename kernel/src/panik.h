#ifndef __PANIK_H__
#define __PANIK_H__
#include <stdint.h>

//These errors are the errors that shut the kernel down completely

//Fatal exceptions
#define ERR_DEVICE_NOT_AVAILABLE                0x000000000
#define ERR_DOUBLE_FAULT                        0x000000001
#define ERR_INVALID_TSS                         0x000000002
#define ERR_SEGMENT_NOT_PRESENT                 0x000000003
#define ERR_STACK_SEGMENT_FAULT                 0x000000004
#define ERR_MACHINE_CHECK                       0x000000005
#define ERR_OUTDATED_OR_RESERVED_INT            0x000000006
#define ERR_IN_KERNEL_PAGE_FAULT                0x000000007

//Process exceptions
#define ERR_NO_PROCESS_TO_SCHEDULE              0x000000010
#define ERR_PROCESS_IS_NOT_A_PROCESS            0x000000011
#define ERR_PROCESS0_KILLED                     0x000000012

//Memory exceptions
#define ERR_OUT_OF_MEMORY                       0x000000050

//Registery exceptions
#define ERR_NO_REGISTERY_FILE                   0x000000065
#define ERR_CORRUPTED_REGISTERY                 0x000000066

//INIT exceptions
#define ERR_NO_ROOT_FILESYSTEM                  0x000000069
#define ERR_NO_INIT_KEY_IN_REGISTERY            0x00000006A
#define ERR_REQUIRED_INIT_STEP_FAILED           0x00000006B

void panik(uint32_t code);

#endif // __PANIK_H__
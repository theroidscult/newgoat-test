#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

#define PIT_COUNTER0                    0x40
#define PIT_CMD                         0x43

#define CONFIG                          0b00110110 //0x36
#define PIT_FREQUENCY                   1193182

void pit_start(uint32_t freq);

#endif // __PIT_H__
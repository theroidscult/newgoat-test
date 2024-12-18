#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>

#define PIT_COUNTER0                    0x40
#define PIT_CMD                         0x43
#define CMD_BINARY                      0x00
#define CMD_BCD                         0x01
#define CMD_MODE0                       0x00
#define CMD_MODE1                       0x02
#define CMD_MODE2                       0x04
#define CMD_MODE3                       0x06
#define CMD_MODE4                       0x08
#define CMD_MODE5                       0x0a
#define CMD_LATCH                       0x00
#define CMD_RW_LOW                      0x10
#define CMD_RW_HI                       0x20
#define CMD_RW_BOTH                     0x30
#define CMD_COUNTER0                    0x00
#define CMD_COUNTER1                    0x40
#define CMD_COUNTER2                    0x80
#define CMD_READBACK                    0xc0
#define PIT_FREQUENCY                   1193182

void pit_start(uint32_t freq);

#endif // __PIT_H__
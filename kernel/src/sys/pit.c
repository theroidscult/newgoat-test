#include "pit.h"
#include <KrnlAid/arch/x86/portio.h>
#include <stdint.h>

void pit_start(uint32_t freq) {
    uint32_t div = PIT_FREQUENCY / freq;
    outb(PIT_CMD, CMD_BINARY | CMD_MODE2 | CMD_RW_BOTH | CMD_COUNTER0);
    outb(PIT_COUNTER0, div & 0xFF);
    outb(PIT_COUNTER0, div >> 8);
}
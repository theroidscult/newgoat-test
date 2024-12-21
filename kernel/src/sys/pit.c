#include "pit.h"
#include <KrnlAid/arch/x86/portio.h>
#include <stdint.h>

void pit_start(uint32_t freq) {
    uint32_t div = PIT_FREQUENCY / freq;
    outb(PIT_CMD, CONFIG);
    outb(PIT_COUNTER0, div);
    outb(PIT_COUNTER0, div >> 8);
}
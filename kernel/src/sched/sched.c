#include <sys/pic.h>
#include <dev/cereal.h>

void sys_timer_isr() {
    cereal_write(0x3F8, 'a');
    pic_eoi(0);
}
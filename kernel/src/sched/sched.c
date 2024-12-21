#include <sys/pic.h>
#include <dev/cereal.h>
#include <sys/context.h>

void sys_timer_isr(context_t* context) {
    cereal_write(0x3F8, 'a');
    pic_eoi(0);
}
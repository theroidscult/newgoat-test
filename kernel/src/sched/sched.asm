global timer_isr
extern sys_timer_isr

timer_isr:
    pushaq
    call sys_timer_isr
    popaq
    iretq
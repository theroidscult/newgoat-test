#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

void idt_set_irq(int irq, void (*handler)(void), uint8_t ist);

#endif // __IDT_H__
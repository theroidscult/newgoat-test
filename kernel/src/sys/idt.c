#include <KrnlAid/arch/x86/idt.h>

extern void err_div();
extern void err_invalid_opcode();
extern void err_gp();
extern void err_pf();
extern void err_fp();
extern void err_simd();

extern void crit_device_not_available();
extern void crit_double_fault();
extern void crit_invalid_tss();
extern void crit_seg_not_present();
extern void crit_stack_segment();
extern void crit_machine_check();
extern void crit_outdated_or_reserved();

extern void dbg_debug();
extern void dbg_brakpoint();

extern void ign_overflow();
extern void ign_range_exceeded();
extern void ign_nmi();
extern void ign_alignment();
extern void ign_virt_stuff();
extern void ign_unregistered_isr();

idt_entry_t entries[256] = {{0}};

void prepare_idt() {

    idt_set_gate(&entries[0x0], err_div, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x1], dbg_debug, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x2], ign_nmi, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x3], dbg_brakpoint, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x4], ign_overflow, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x5], ign_range_exceeded, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x6], err_invalid_opcode, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x7], crit_device_not_available, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x8], crit_double_fault, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x9], crit_outdated_or_reserved, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0xA], crit_invalid_tss, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0xB], crit_seg_not_present, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0xC], crit_stack_segment, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0xD], err_gp, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0xE], err_pf, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0xF], crit_outdated_or_reserved, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x10], err_fp, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x11], ign_alignment, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x12], crit_machine_check, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x13], err_simd, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x14], ign_virt_stuff, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    for (int i = 0x15; i <= 0x1B; i++) {
        idt_set_gate(&entries[i], crit_outdated_or_reserved, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    }
    idt_set_gate(&entries[0x1C], ign_virt_stuff, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x1D], ign_virt_stuff, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x1E], ign_virt_stuff, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    idt_set_gate(&entries[0x1F], crit_outdated_or_reserved, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);

    for(int i = 0x20; i <= 0xFF; i++) {
        idt_set_gate(&entries[i], ign_unregistered_isr, 0x08, 0, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
    }

    idt_pointer_t pointer = make_idt_pointer(entries, 256);
    load_idt(&pointer);
}

void idt_set_irq(int irq, void (*handler)(void), uint8_t ist) {
    idt_set_gate(&entries[32+irq], handler, 0x08, ist, IDT_TYPE_INTERRUPT | IDT_ACCESS_PRESENT);
}
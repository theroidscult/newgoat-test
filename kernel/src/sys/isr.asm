global err_div
global err_invalid_opcode
global err_gp
global err_pf
global err_fp
global err_simd

global crit_device_not_available
global crit_double_fault
global crit_invalid_tss
global crit_seg_not_present
global crit_stack_segment
global crit_machine_check
global crit_outdated_or_reserved

global dbg_debug
global dbg_brakpoint

global ign_overflow
global ign_range_exceeded
global ign_nmi
global ign_alignment
global ign_virt_stuff

%macro pushaq 0
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push rsp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsp
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax
%endmacro

; user errors(probably)
err_div:
    pushaq
    ; TODO: Do something
    popaq
 
    iretq

err_invalid_opcode:
    pushaq
    ; TODO: Do something
    popaq

    iretq

err_gp:
    pushaq
    ; TODO: Do something
    popaq

    iretq

err_pf:
    pushaq
    ; TODO: Do something
    popaq

    iretq

err_fp:
    pushaq
    ; TODO: Do something
    popaq

    iretq

err_simd:
    pushaq
    ; TODO: Do something
    popaq

    iretq

; crirical errors(instant kernel panic)
crit_device_not_available:
    jmp $;TODO: add some kind of BSOD

crit_double_fault:
    jmp $;TODO: add some kind of BSOD

crit_invalid_tss:
    jmp $;TODO: add some kind of BSOD

;NOTE: should not happen on 64 bit
crit_seg_not_present:
    jmp $;TODO: add some kind of BSOD

crit_stack_segment:
    jmp $;TODO: add some kind of BSOD

;NOTE: should not happen
crit_machine_check:
    jmp $;TODO: add some kind of BSOD

;NOTE: these are unnused but if something goes wrong it is still here
crit_outdated_or_reserved:
    jmp $;TODO: add some kind of BSOD

;debug stuff
dbg_debug:
    ;TODO: add debugger support
    iretq

dbg_brakpoint:
    ;TODO: add debugger support
    iretq

;ignore
ign_overflow:
    iretq

ign_range_exceeded:
    iretq

ign_nmi:
    ;TODO: add ability for drivers to handle the NMI
    iretq

ign_alignment:
    ;TODO: add abilitiy to enable the alignment check for a process
    iretq

ign_virt_stuff:
    iretq
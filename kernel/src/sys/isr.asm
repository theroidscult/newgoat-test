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

extern panik

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
    mov rdi, 0
    call panik

crit_double_fault:
    mov rdi, 1
    call panik

crit_invalid_tss:
    mov rdi, 2
    call panik

;NOTE: should not happen on 64 bit
crit_seg_not_present:
    mov rdi, 3
    call panik

crit_stack_segment:
    mov rdi, 4
    call panik

;NOTE: should not happen
crit_machine_check:
    mov rdi, 5
    call panik

;NOTE: these are unnused but if something goes wrong it is still here
crit_outdated_or_reserved:
    mov rdi, 6
    call panik

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
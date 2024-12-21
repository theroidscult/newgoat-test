global timer_isr
extern sys_timer_isr

%macro pushaq 0
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
%endmacro

%macro popaq 0
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
%endmacro

timer_isr:
    push 0

    pushaq
    mov rdi, rsp
    call sys_timer_isr
    popaq

    add rsp, 8

    iretq
global _start
extern printf, scanf, pow, fflush, stdout
section .bss

inputbuf: resq 8

section .data

in_str:  db "%d"             ; format string for scanf
out_str: db ">> %d.%d", 0xA ; format string for printf

section .text

_start:
	push rbx   ; push everything
	push rbp   ; push everything
	push r12   ; push everything
	push r13   ; push everything
	push r14   ; push everything
	push r15   ; push everything

	call f1058 ; call main

	pop rbx   ; restore initial regs state
	pop rbp   ; restore initial regs state
	pop r12   ; restore initial regs state
	pop r13   ; restore initial regs state
	pop r14   ; restore initial regs state
	pop r15   ; restore initial regs state

	mov rax, 0x3C
	xor rdi, rdi
	syscall

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 0 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьБ; [8; 16]
	mov [rbp - 8], rax ; ЛошедьБ = rax
	
	mov rax, 4608 ; const value << 9
	mov [rbp - 8], rax ; ЛошедьБ = rax
	
	mov rax, [rbp - 8] ; ЛошедьБ
	lea rdi, [out_str]

	mov rsi, rax
	shr rsi, 9 ; pseudo-float emul
	and rax, 511 ; mask for last 9 bits
	mov rcx, 3E8h ; rcx = 1000d for translation to base 10
	mul rcx
	shr rax, 9
	mov rdx, rax

	xor rax, rax
	call printf
	mov rdi, [stdout]
	call fflush
	
	mov rax, [rbp - 8] ; ЛошедьБ
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	


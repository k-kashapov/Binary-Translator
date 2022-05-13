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

f7639: ; def Функция
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 16 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared Разность; [24; 32]
	mov [rbp - 24], rax ; Разность = rax
	
		mov rax, [rbp - 16] ; Правый
		push rax

		mov rax, [rbp - 8] ; Левый
		pop rbx

		sub rax, rbx

	mov [rbp - 24], rax ; Разность = rax
	
	mov rax, [rbp - 24] ; Разность
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 0 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьА; [8; 16]
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	mov rax, 3072 ; const value << 9
	sub rsp, 8 ; declared Шесть; [16; 24]
	mov [rbp - 16], rax ; Шесть = rax
	
	mov rax, 1024 ; const value << 9
	sub rsp, 8 ; declared Два; [24; 32]
	mov [rbp - 24], rax ; Два = rax
	
	; call args
	mov rax, [rbp - 16] ; Шесть
	mov [rsp - 24], rax
	mov rax, [rbp - 24] ; Два
	mov [rsp - 32], rax
	call f7639 ; call Функция
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	


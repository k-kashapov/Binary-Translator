global _start
extern printf, scanf, pow, fflush, stdout
section .bss

inputbuf: resq 8

section .data

in_str:  db "%d"                         ; format string for scanf
out_str: db ">> %d + %d / 512", 0xA ; format string for printf

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

f11528: ; def Факториал
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 8 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared Результат; [16; 24]
	mov [rbp - 16], rax ; Результат = rax
	
	; if statement
		; ja
			mov rax, [rbp - 8] ; Итератор
			mov rbx, rax ; save left to rbx
			mov rax, 512 ; const value << 9
			cmp rbx, rax
			ja .0cmp

			xor rax, rax ; false
			jmp .0cmpEnd

			.0cmp:
			mov rax, 1 ; true

			.0cmpEnd:

		test rax, rax
		je .0false

		; call args
			mov rax, 512 ; const value << 9
			push rax

			mov rax, [rbp - 8] ; Итератор
			pop rbx

			sub rax, rbx

		mov [rsp - 24], rax
		call f11528 ; call Факториал
		mov [rbp - 16], rax ; Результат = rax
		
		jmp .0enif

		.0false:

		mov rax, 512 ; const value << 9
		mov [rbp - 16], rax ; Результат = rax
		
		.0enif:

	
		mov rax, [rbp - 8] ; Итератор
		push rax

		mov rax, [rbp - 16] ; Результат
		pop rbx

		mul rbx

		shr rax, 9 ; pseudo-float emul
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 0 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared Рез; [8; 16]
	mov [rbp - 8], rax ; Рез = rax
	
	mov rax, 3072 ; const value << 9
	sub rsp, 8 ; declared Шесть; [16; 24]
	mov [rbp - 16], rax ; Шесть = rax
	
	; call args
	mov rax, [rbp - 16] ; Шесть
	mov [rsp - 24], rax
	call f11528 ; call Факториал
	mov [rbp - 8], rax ; Рез = rax
	
	mov rax, [rbp - 8] ; Рез
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	


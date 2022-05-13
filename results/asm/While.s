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

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 0 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьА; [8; 16]
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьБ; [16; 24]
	mov [rbp - 16], rax ; ЛошедьБ = rax
	
	mov rax, 5120 ; const value << 9
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	mov rax, 3072 ; const value << 9
	mov [rbp - 16], rax ; ЛошедьБ = rax
	
	mov r12, rsp ; save rsp to rcx
	; while
		.0while:
		; ja
			mov rax, [rbp - 8] ; ЛошедьА
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
		je .0whileEnd
		mov rax, 0 ; const value << 9
		sub rsp, 8 ; declared Временно; [24; 32]
		mov [rbp - 24], rax ; Временно = rax
		
			mov rax, 512 ; const value << 9
			push rax

			mov rax, [rbp - 24] ; Временно
			pop rbx

			add rax, rbx

		mov [rbp - 24], rax ; Временно = rax
		
			mov rax, [rbp - 24] ; Временно
			push rax

			mov rax, [rbp - 16] ; ЛошедьБ
			pop rbx

			add rax, rbx

		mov [rbp - 16], rax ; ЛошедьБ = rax
		
			mov rax, 512 ; const value << 9
			push rax

			mov rax, [rbp - 8] ; ЛошедьА
			pop rbx

			sub rax, rbx

		mov [rbp - 8], rax ; ЛошедьА = rax
		
		mov rsp, r12 ; forget any variables created during the loop
		jmp .0while
		.0whileEnd:
	
	mov rax, [rbp - 16] ; ЛошедьБ
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	


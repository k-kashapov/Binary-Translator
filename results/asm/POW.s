global _start
extern printf, scanf, pow, fflush, stdout
section .bss

inputbuf: resq 2

section .data

const_for_pow: dd 0x200        ; memory for float computations
in_str:  db "%d"             ; format string for scanf
out_str: db ">> %d.%d", 0xA  ; format string for printf

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

	mov rdi, rax
	mov rax, 0x3C
	syscall

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 0 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 16 ; declared ЛошедьА; [16; 32]
	mov [rbp - 16], rax ; ЛошедьА = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 16 ; declared ЛошедьБ; [32; 48]
	mov [rbp - 32], rax ; ЛошедьБ = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 16 ; declared ЛошедьВ; [48; 64]
	mov [rbp - 48], rax ; ЛошедьВ = rax
	
	mov rdi, in_str   ; format string for float value
	mov rsi, inputbuf ; buffer for inputted value

	mov rax, 0
	push rbp
	mov rbp, rsp

	sub rsp, 8
	call scanf

	mov rax, [inputbuf]
	shl rax, 9 ; pseudo-float emul
	mov rsp, rbp
	pop rbp

	mov [rbp - 16], rax ; ЛошедьА = rax
	
	mov rdi, in_str   ; format string for float value
	mov rsi, inputbuf ; buffer for inputted value

	mov rax, 0
	push rbp
	mov rbp, rsp

	sub rsp, 8
	call scanf

	mov rax, [inputbuf]
	shl rax, 9 ; pseudo-float emul
	mov rsp, rbp
	pop rbp

	mov [rbp - 32], rax ; ЛошедьБ = rax
	
		mov rax, [rbp - 32] ; ЛошедьБ
		push rax

		mov rax, [rbp - 16] ; ЛошедьА
		push rax

		fild  WORD [rsp + 16]      ; load power onto FPU stack
		fidiv DWORD [const_for_pow] ; convert from pseudo-float

		fild  WORD [rsp]           ; load base onto FPU stack
		fidiv DWORD [const_for_pow] ; convert from pseudo-float

		fyl2x ; power * log_2_(base)

		; value between -1 and 1 is required by pow of 2 command
		fist DWORD [rsp - 8] ; cast to int
		fild DWORD [rsp - 8] ;
		fsub      ; fit into [-1; 1]

		f2xm1 ; 2^(power * log_2_(base)) - 1 = base^power

		fld1   ; push 1
		fadd   ; add 1 to the result

		fild DWORD [rsp - 8] ; load casted value
		fxch   ; exchange st(0) <-> st(1)
		fscale ; multiply by remaining power of 2
		fimul DWORD [const_for_pow] ; to pseudo-float
		fistp DWORD [rsp + 16]      ; save pow value to stack

		add rsp, 16
		pop rax
	mov [rbp - 48], rax ; ЛошедьВ = rax
	
	mov rax, [rbp - 48] ; ЛошедьВ
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

	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	


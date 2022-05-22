global _start
section .data

const_for_pow: dd 0x200        ; memory for float computations
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
	sub rsp, 8 ; declared ЛошедьА; [8; 16]
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьБ; [16; 24]
	mov [rbp - 16], rax ; ЛошедьБ = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьВ; [24; 32]
	mov [rbp - 24], rax ; ЛошедьВ = rax
	
	xor rdi, rdi
	sub rsp, 8
	mov rsi, rsp ; buffer for inputted value

	mov rdx, 7
	xor rax, rax
	syscall
	mov rsi, rsp
	mov rcx, rax
	call atoi
	add rsp, 8
	sal rax, 9 ; pseudo-float emul
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	xor rdi, rdi
	sub rsp, 8
	mov rsi, rsp ; buffer for inputted value

	mov rdx, 7
	xor rax, rax
	syscall
	mov rsi, rsp
	mov rcx, rax
	call atoi
	add rsp, 8
	sal rax, 9 ; pseudo-float emul
	mov [rbp - 16], rax ; ЛошедьБ = rax
	
		mov rax, [rbp - 8] ; ЛошедьА
		test rax, rax
		jz .DontPow
		cmp rax, 1
		je .DontPow
		push rax

		mov rax, [rbp - 16] ; ЛошедьБ
		cmp rax, 1
		je .DontPowButPop
		push rax

		fild  WORD [rsp] ; load base onto FPU stack
		push 512
		fidiv WORD [rsp] ; convert from pseudo-float

		fild  WORD [rsp + 16] ; load power onto FPU stack
		fidiv WORD [rsp]      ; convert from pseudo-float

		add rsp, 8  ; remove 512 from stack
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
		fistp DWORD [rsp + 8]      ; save pow value to stack

		add rsp, 8
		.DontPowButPop:
		pop rax
		.DontPow:
	mov [rbp - 24], rax ; ЛошедьВ = rax
	
	mov rax, [rbp - 24] ; ЛошедьВ
	call out
	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

section .text

;==============================================
; StdLib: out
; Expects:
;   rax - value
; Returns: None
; Corrupt:
;   rdi = rsp - Will create buffer on stack
;==============================================

out:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov rdi, rsp
    mov BYTE [rdi], 0x0A ; \n
    inc rdi
    mov WORD [rdi], ">>"; \n
    add rdi, 2
    mov BYTE [rdi], 0x20 ; \n
    inc rdi
    cmp rax, 0
    jge .NotNegative
    mov BYTE [rdi], '-'
    inc rdi
    neg rax
.NotNegative:
    push rax
    shr rax, 9
    mov rdx, rax
    mov r10, 10
    call CntBytes
    call itoa10
    pop rax
    add rdi, r8 ; step over the word
    mov BYTE [rdi], 0x2E ; \n
    inc rdi
    and rax, 511
    mov rbx, 1000
    mul rbx
    shr rax, 9
    mov rdx, rax
    mov WORD [rdi], 0x30303030
    add rdi, 2
    call itoa10
    mov rax, 0x01 ; write
    mov rdx, rdi ; buffer len
    sub rdx, rsp ; buffer len
    inc rdx      ; buffer len
    mov rdi, 0x01 ; stdout
    mov rsi, rsp ; buffer start
    syscall
    mov rsp, rbp
    pop rbp
    ret
;==============================================
; StdLib: itoa
;==============================================

;==============================================
; Count bytes
; Counts the amount of bytes needed to write down an int
; Expects:
;      rax - value
;      r10 = 10
;      rdi - buffer
;==============================================

CntBytes:              ; skips, bytes that are required to save the value
xor r8, r8         ; reset bytecount
push rax
.loop:xor rdx, rdx		; reset remaining
div r10            ; rax = rax / 10; rdx = rax % 10
inc rdi
inc r8
cmp rax, 0
ja .loop
mov rax, r9           	; reset value
mov byte [rdi], 00
dec rdi
pop rax
ret
;==============================================
; Converts integer value into a string, base 10
; Expects:
;       r10 = 10
;       rdx - Integer value
;       rdi - Buffer to write into
; Returns:
;       r8  - Printed bytes num
; Destr:
;       rdx, r10, r9
;==============================================
itoa10:
xor r8, r8		    ; r8 = bytes counter
.Print:
xor rdx, rdx
div r10                ; rax = rax / 10; rdx = rax % 10
add dl, '0'           	; to ASCII
mov [rdi], dl
dec rdi
inc r8
cmp rax, 00h
ja .Print
; rdi = &buffer - 1
inc rdi ; rdi = &buffer
ret


section .bss

inputbuf: resq 2

section .text

;==============================================
; StdLib: atoi
; Expects:
;     rsi = rsp - input buffer
;     rcx - len of input
; Returns:
;     rax - result int
;==============================================
atoi:
    xor rax, rax
    xor rbx, rbx
    dec rcx
    jz .End
    cmp BYTE [rsi], '-'
    jne .Loop
    inc rsi
    dec rcx
.Loop:
    mov bl, [rsi]
    sub rbx, '0'
    inc rsi
    mov rdx, 10
    mul rdx
    add rax, rbx
    loop .Loop
.End:    cmp BYTE [rsp + 8], '-'
    jne .Ret
    neg rax
.Ret:
    ret



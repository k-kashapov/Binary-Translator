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
	mov rsi, inputbuf ; buffer for inputted value

	mov rdx, 15
	xor rax, rax
	syscall
	mov rcx, rax
	call atoi
	sal rax, 9 ; pseudo-float emul
	mov [rbp - 8], rax ; ЛошедьА = rax
	
	xor rdi, rdi
	mov rsi, inputbuf ; buffer for inputted value

	mov rdx, 15
	xor rax, rax
	syscall
	mov rcx, rax
	call atoi
	sal rax, 9 ; pseudo-float emul
	mov [rbp - 16], rax ; ЛошедьБ = rax
	
		mov rax, 1024 ; const value << 9
		push rax

			mov rax, [rbp - 8] ; ЛошедьА
			test rax, rax
			jz .DontPow
			cmp rax, 1
			je .DontPow
			push rax

			mov rax, 512 ; const value << 9
			cmp rax, 1
			je .DontPowButPop
			push rax

			fild  WORD [rsp]            ; load base onto FPU stack
			fidiv DWORD [const_for_pow] ; convert from pseudo-float

			fild  WORD [rsp + 8]      ; load power onto FPU stack
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
			fistp DWORD [rsp + 8]      ; save pow value to stack

			add rsp, 8
			.DontPowButPop:
			pop rax
			.DontPow:
		pop rbx

		sar rbx, 9 ; pseudo-float emul
		cqo

		idiv rbx

	mov [rbp - 24], rax ; ЛошедьВ = rax
	
	mov rax, [rbp - 24] ; ЛошедьВ
	call out
	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

section .data

outArr: db 10, ">> "
outBig: dq 0, 0
outDot: db '.'
outLow: dq 0, 0 

section .text

;==============================================
; StdLib: out
; Expects:
;   outbuffer
;   rax - value
; Returns: None
;==============================================

out:
    mov rdi, outBig
    cmp rax, 0
    jge .NotNegative
    mov BYTE [outBig], '-'
    inc rdi
    neg rax
.NotNegative:
    push rax
    shr rax, 9
    mov rdx, rax
    call itoa10
    mov rax, 0x01
    mov rdi, 0x01
    mov rsi, outArr
    mov rdx, r8
    add rdx, 5
    syscall
    pop rax
    and rax, 511
    mov rbx, 1000
    mul rbx
    shr rax, 9
    mov rdx, rax
    mov rdi, outLow
    call itoa10
    mov rax, 0x01
    mov rdi, 0x01
    mov rsi, outDot
    mov rdx, r8
    add rdx, 1
    syscall
    ret
;==============================================
; StdLib: itoa
;==============================================

CountBytes:
	xor rax, rax
        mov rax, rdx	; save value in r10 to count symbols in it
        xor ch, ch
.Loop:
        inc ch  	; bytes counter
        shr rax, cl     ; rax >> cl
        jnz .Loop
	xor rax, rax
        mov al, ch
ret
;==============================================
; Converts integer value into a string, base 10
; Expects:
;       rdx - Integer value
;       rdi - Buffer to write into
; Returns:
;       r8  - Printed bytes num
; Destr:
;       rdx, r10, r9
;==============================================
itoa10:
xor r8, r8		; r8 = bytes counter
mov r9, rdx 		; from now on, value is stored in r9
mov rax, rdx		; save value to rax
mov r10, 10
.CntBytes:              	; skips, bytes that are required to save the value
xor rdx, rdx		; reset remaining
div r10            ; rax = rax / 10; rdx = rax % 10
inc rdi
inc r8
cmp rax, 0000h
ja .CntBytes
mov rax, r9           	; reset value
mov byte [rdi], 00
dec rdi
.Print:
xor rdx, rdx
div r10                ; rax = rax / 10; rdx = rax % 10
add dl, '0'           	; to ASCII
mov [rdi], dl
dec rdi
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
;     rsi - inputbuf
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
.End:    cmp BYTE [inputbuf], '-'
    jne .Ret
    neg rax
.Ret:
    ret



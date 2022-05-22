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
	sub rsp, 8 ; declared ЛошедьБ; [8; 16]
	mov [rbp - 8], rax ; ЛошедьБ = rax
	
		mov rax, 262144 ; const value << 9
		push rax

		mov rax, 261632 ; const value << 9
		pop rbx

		sar rbx, 9 ; pseudo-float emul
		cqo

		idiv rbx

	mov [rbp - 8], rax ; ЛошедьБ = rax
	
	mov rax, [rbp - 8] ; ЛошедьБ
	call out
	
	mov rax, [rbp - 8] ; ЛошедьБ
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
    push r8
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
    mov rdi, 0x01 ; stdout
    mov rsi, rsp ; buffer start
    add rsi, 8
    mov rdx, r8 ; buffer len computation
    pop r8 ; first part len
    add rdx, r8
    add rdx, 6 ; constant part of the string
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



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
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьФ; [16; 24]
	mov [rbp - 16], rax ; ЛошедьФ = rax
	
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
	mov [rbp - 8], rax ; ЛошедьБ = rax
	
	mov rax, [rbp - 8] ; ЛошедьБ
	call out
	
	mov rax, [rbp - 8] ; ЛошедьБ
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



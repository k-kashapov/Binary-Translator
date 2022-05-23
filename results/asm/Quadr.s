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

f8991: ; def Линейная
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 16 ; jump over parameters

	; if statement
		; je
			mov rax, [rbp - 16] ; ЛошедьБ
			mov rbx, rax ; save left to rbx
			mov rax, 0 ; const value << 9
			cmp rbx, rax
			je .0cmp

			xor rax, rax ; false
			jmp .0cmpEnd

			.0cmp:
			mov rax, 1 ; true

			.0cmpEnd:

		test rax, rax
		je .0false

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value << 9
				cmp rbx, rax
				je .1cmp

				xor rax, rax ; false
				jmp .1cmpEnd

				.1cmp:
				mov rax, 1 ; true

				.1cmpEnd:

			test rax, rax
			je .1false

			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Количество; [24; 32]
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, 51200 ; const value << 9
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			call out
			
			jmp .1enif

			.1false:

			mov rax, 0 ; const value << 9
			mov [rbp - 32], rax ; Количество = rax
			
			mov rax, 512 ; const value << 9
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			call out
			
			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Нулёвочка; [32; 40]
			mov [rbp - 32], rax ; Нулёвочка = rax
			
			mov rax, 0 ; const value << 9
			mov [rbp - 32], rax ; Нулёвочка = rax
			
			mov rax, [rbp - 32] ; Нулёвочка
			call out
			
			.1enif:

		
		jmp .0enif

		.0false:

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value << 9
				cmp rbx, rax
				je .2cmp

				xor rax, rax ; false
				jmp .2cmpEnd

				.2cmp:
				mov rax, 1 ; true

				.2cmpEnd:

			test rax, rax
			je .2false

			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Жопа; [40; 48]
			mov [rbp - 40], rax ; Жопа = rax
			
				mov rax, 512 ; const value << 9
				push rax

				mov rax, 0 ; const value << 9
				pop rbx

				sub rax, rbx

			mov [rbp - 40], rax ; Жопа = rax
			
			mov rax, [rbp - 40] ; Жопа
			call out
			
			jmp .2enif

			.2false:

			mov rax, 0 ; const value << 9
			mov [rbp - 32], rax ; Количество = rax
			
			mov rax, 512 ; const value << 9
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			call out
			
			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Результат; [48; 56]
			mov [rbp - 48], rax ; Результат = rax
			
					mov rax, [rbp - 8] ; ЛошедьК
					push rax

					mov rax, [rbp - 16] ; ЛошедьБ
					pop rbx

					sar rbx, 9 ; pseudo-float emul
					cqo

					idiv rbx

				push rax

				mov rax, 0 ; const value << 9
				pop rbx

				sub rax, rbx

			mov [rbp - 48], rax ; Результат = rax
			
			mov rax, [rbp - 48] ; Результат
			call out
			
			.2enif:

		
		.0enif:

	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

f7938: ; def Квадрат
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 24 ; jump over parameters

	; if statement
		; je
			mov rax, [rbp - 8] ; ЛошедьА
			mov rbx, rax ; save left to rbx
			mov rax, 0 ; const value << 9
			cmp rbx, rax
			je .3cmp

			xor rax, rax ; false
			jmp .3cmpEnd

			.3cmp:
			mov rax, 1 ; true

			.3cmpEnd:

		test rax, rax
		je .3false

		; call args
		mov rax, [rbp - 16] ; ЛошедьБ
		mov [rsp - 24], rax
		mov rax, [rbp - 24] ; ЛошедьВ
		mov [rsp - 32], rax
		call f8991 ; call Линейная
		mov [rbp - 8], rax ; ЛошедьА = rax
		
		jmp .3enif

		.3false:

		mov rax, 0 ; const value << 9
		sub rsp, 8 ; declared Дискриминант; [32; 40]
		mov [rbp - 32], rax ; Дискриминант = rax
		
					mov rax, [rbp - 24] ; ЛошедьВ
					push rax

					mov rax, [rbp - 8] ; ЛошедьА
					pop rbx

					imul rbx

					sar rax, 9 ; pseudo-float emul
				push rax

				mov rax, 2048 ; const value << 9
				pop rbx

				imul rbx

				sar rax, 9 ; pseudo-float emul
			push rax

				mov rax, [rbp - 16] ; ЛошедьБ
				push rax

				mov rax, [rbp - 16] ; ЛошедьБ
				pop rbx

				imul rbx

				sar rax, 9 ; pseudo-float emul
			pop rbx

			sub rax, rbx

		mov [rbp - 32], rax ; Дискриминант = rax
		
		mov rax, [rbp - 32] ; Дискриминант
		call out
		
		; if statement
			; jge
				mov rax, [rbp - 32] ; Дискриминант
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value << 9
				cmp rbx, rax
				jge .4cmp

				xor rax, rax ; false
				jmp .4cmpEnd

				.4cmp:
				mov rax, 1 ; true

				.4cmpEnd:

			test rax, rax
			je .4false

			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Количество; [40; 48]
			mov [rbp - 40], rax ; Количество = rax
			
			mov rax, 1024 ; const value << 9
			mov [rbp - 40], rax ; Количество = rax
			
			mov rax, [rbp - 40] ; Количество
			call out
			
			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Корень; [48; 56]
			mov [rbp - 48], rax ; Корень = rax
			
				mov rax, [rbp - 32] ; Дискриминант
				push rax

					mov rax, 1024 ; const value << 9
					push rax

					mov rax, 512 ; const value << 9
					pop rbx

					sar rbx, 9 ; pseudo-float emul
					cqo

					idiv rbx

				push rax

				mov rax, [rsp + 8]
				test rax, rax
				jz .DontPow
				cmp rax, 1
				je .DontPow
				cmp WORD [rsp], 1
				je .DontPowButPop
				fild  WORD [rsp]      ; load base onto FPU stack
				push 512
				fidiv WORD [rsp]      ; convert from pseudo-float

				fild  WORD [rsp + 16] ; load power onto FPU stack
				fidiv WORD [rsp]      ; convert from pseudo-float

				fyl2x                 ; power * log_2_(base)

				; value between -1 and 1 is required by pow of 2 command
				fist DWORD [rsp - 8]  ; cast to int
				fild DWORD [rsp - 8]  ;
				fsub                  ; fit into [-1; 1]

				f2xm1                 ; 2^(power * log_2_(base)) - 1 = base^power

				fld1                  ; push 1
				fadd                  ; add 1 to the result

				fild DWORD [rsp - 8]  ; load casted value
				fxch                  ; exchange st(0) <-> st(1)
				fscale                ; multiply by remaining power of 2
				fimul DWORD [rsp]      ; to pseudo-float
				fistp DWORD [rsp + 16] ; save pow value to stack

				add rsp, 16           ; remove 512 and args from stack
				.DontPowButPop:
				pop rax
				.DontPow:
			mov [rbp - 48], rax ; Корень = rax
			
			mov rax, 0 ; const value << 9
			sub rsp, 16 ; declared Корни; [56; 72]
			mov [rbp - 64], rax ; Корни = rax
			
					mov rax, 1024 ; const value << 9
					push rax

					mov rax, [rbp - 8] ; ЛошедьА
					pop rbx

					imul rbx

					sar rax, 9 ; pseudo-float emul
				push rax

					mov rax, [rbp - 16] ; ЛошедьБ
					push rax

					mov rax, [rbp - 48] ; Корень
					pop rbx

					sub rax, rbx

				pop rbx

				sar rbx, 9 ; pseudo-float emul
				cqo

				idiv rbx

			mov [rbp - 56], rax ; Корни = rax
			
					mov rax, 1024 ; const value << 9
					push rax

					mov rax, [rbp - 8] ; ЛошедьА
					pop rbx

					imul rbx

					sar rax, 9 ; pseudo-float emul
				push rax

						mov rax, [rbp - 16] ; ЛошедьБ
						push rax

						mov rax, [rbp - 48] ; Корень
						pop rbx

						add rax, rbx

					push rax

					mov rax, 0 ; const value << 9
					pop rbx

					sub rax, rbx

				pop rbx

				sar rbx, 9 ; pseudo-float emul
				cqo

				idiv rbx

			mov [rbp - 64], rax ; Корни = rax
			
			mov rax, [rbp - 56] ; Корни
			call out
			
			mov rax, [rbp - 64] ; Корни
			call out
			
			jmp .4enif

			.4false:

			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Жопа; [72; 80]
			mov [rbp - 72], rax ; Жопа = rax
			
				mov rax, 512 ; const value << 9
				push rax

				mov rax, 0 ; const value << 9
				pop rbx

				sub rax, rbx

			mov [rbp - 72], rax ; Жопа = rax
			
			mov rax, [rbp - 72] ; Жопа
			call out
			
			.4enif:

		
		.3enif:

	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 0 ; jump over parameters

	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared Костыль; [8; 16]
	mov [rbp - 8], rax ; Костыль = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьА; [16; 24]
	mov [rbp - 16], rax ; ЛошедьА = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьБ; [24; 32]
	mov [rbp - 24], rax ; ЛошедьБ = rax
	
	mov rax, 0 ; const value << 9
	sub rsp, 8 ; declared ЛошедьВ; [32; 40]
	mov [rbp - 32], rax ; ЛошедьВ = rax
	
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
	mov [rbp - 16], rax ; ЛошедьА = rax
	
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
	mov [rbp - 24], rax ; ЛошедьБ = rax
	
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
	mov [rbp - 32], rax ; ЛошедьВ = rax
	
	; call args
	mov rax, [rbp - 16] ; ЛошедьА
	mov [rsp - 24], rax
	mov rax, [rbp - 24] ; ЛошедьБ
	mov [rsp - 32], rax
	mov rax, [rbp - 32] ; ЛошедьВ
	mov [rsp - 40], rax
	call f7938 ; call Квадрат
	mov [rbp - 8], rax ; Костыль = rax
	
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



global _start
extern printf, scanf, pow, fflush, stdout
section .bss

inputbuf: resq 2

section .data

const_for_pow: dd 0x200        ; memory for float computations
in_str:  db "%d"             ; format string for scanf
out_str: db ">> %d.%d", 0xA  ; format string for printf

section .text

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
			je .0cmp

			xor rax, rax ; false
			jmp .0cmpEnd

			.0cmp:
			mov rax, 1 ; true

			.0cmpEnd:

		test rax, rax
		je .0false

		; call args
		mov rax, [rbp - 16] ; ЛошедьБ
		mov [rsp - 24], rax
		mov rax, [rbp - 24] ; ЛошедьВ
		mov [rsp - 32], rax
		call f8991 ; call Линейная
		mov [rbp - 8], rax ; ЛошедьА = rax
		
		jmp .0enif

		.0false:

		mov rax, 0 ; const value << 9
		sub rsp, 8 ; declared Дискриминант; [32; 40]
		mov [rbp - 32], rax ; Дискриминант = rax
		
					mov rax, [rbp - 24] ; ЛошедьВ
					push rax

					mov rax, [rbp - 8] ; ЛошедьА
					pop rbx

					imul rbx

					shr rax, 9 ; pseudo-float emul
				push rax

				mov rax, 2048 ; const value << 9
				pop rbx

				imul rbx

				shr rax, 9 ; pseudo-float emul
			push rax

				mov rax, [rbp - 16] ; ЛошедьБ
				push rax

				mov rax, [rbp - 16] ; ЛошедьБ
				pop rbx

				imul rbx

				shr rax, 9 ; pseudo-float emul
			pop rbx

			sub rax, rbx

		mov [rbp - 32], rax ; Дискриминант = rax
		
		mov rax, [rbp - 32] ; Дискриминант
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

		
		; if statement
			; jge
				mov rax, [rbp - 32] ; Дискриминант
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value << 9
				cmp rbx, rax
				jge .1cmp

				xor rax, rax ; false
				jmp .1cmpEnd

				.1cmp:
				mov rax, 1 ; true

				.1cmpEnd:

			test rax, rax
			je .1false

			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Количество; [40; 48]
			mov [rbp - 40], rax ; Количество = rax
			
			mov rax, 1024 ; const value << 9
			mov [rbp - 40], rax ; Количество = rax
			
			mov rax, [rbp - 40] ; Количество
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
			sub rsp, 8 ; declared Корень; [48; 56]
			mov [rbp - 48], rax ; Корень = rax
			
				mov rax, [rbp - 32] ; Дискриминант
				test rax, rax
				jz .DontPow
				cmp rax, 1
				je .DontPow
				push rax

					mov rax, 1024 ; const value << 9
					push rax

					mov rax, 512 ; const value << 9
					pop rbx

					shr rbx, 9 ; pseudo-float emul
					mov rdx, 0
					div rbx

				cmp rax, 1
				je .DontPowButPop
				push rax

				fild  WORD [rsp + 8]      ; load power onto FPU stack
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
				fistp DWORD [rsp + 8]      ; save pow value to stack

				add rsp, 8
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

					shr rax, 9 ; pseudo-float emul
				push rax

					mov rax, [rbp - 16] ; ЛошедьБ
					push rax

					mov rax, [rbp - 48] ; Корень
					pop rbx

					sub rax, rbx

				pop rbx

				shr rbx, 9 ; pseudo-float emul
				mov rdx, 0
				div rbx

			mov [rbp - 56], rax ; Корни = rax
			
					mov rax, 1024 ; const value << 9
					push rax

					mov rax, [rbp - 8] ; ЛошедьА
					pop rbx

					imul rbx

					shr rax, 9 ; pseudo-float emul
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

				shr rbx, 9 ; pseudo-float emul
				mov rdx, 0
				div rbx

			mov [rbp - 64], rax ; Корни = rax
			
			mov rax, [rbp - 56] ; Корни
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

			
			mov rax, [rbp - 64] ; Корни
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

			
			jmp .1enif

			.1false:

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

			
			.1enif:

		
		.0enif:

	
	mov rax, 0 ; const value << 9
	mov rsp, rbp
	pop rbp ; stack frame return

	ret
	

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
			je .2cmp

			xor rax, rax ; false
			jmp .2cmpEnd

			.2cmp:
			mov rax, 1 ; true

			.2cmpEnd:

		test rax, rax
		je .2false

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
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

			mov rax, 0 ; const value << 9
			sub rsp, 8 ; declared Количество; [24; 32]
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, 51200 ; const value << 9
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
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

			
			jmp .3enif

			.3false:

			mov rax, 0 ; const value << 9
			mov [rbp - 32], rax ; Количество = rax
			
			mov rax, 512 ; const value << 9
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
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
			sub rsp, 8 ; declared Нулёвочка; [32; 40]
			mov [rbp - 32], rax ; Нулёвочка = rax
			
			mov rax, 0 ; const value << 9
			mov [rbp - 32], rax ; Нулёвочка = rax
			
			mov rax, [rbp - 32] ; Нулёвочка
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

			
			.3enif:

		
		jmp .2enif

		.2false:

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value << 9
				cmp rbx, rax
				je .4cmp

				xor rax, rax ; false
				jmp .4cmpEnd

				.4cmp:
				mov rax, 1 ; true

				.4cmpEnd:

			test rax, rax
			je .4false

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

			
			jmp .4enif

			.4false:

			mov rax, 0 ; const value << 9
			mov [rbp - 32], rax ; Количество = rax
			
			mov rax, 512 ; const value << 9
			mov [rbp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
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
			sub rsp, 8 ; declared Результат; [48; 56]
			mov [rbp - 48], rax ; Результат = rax
			
					mov rax, [rbp - 8] ; ЛошедьК
					push rax

					mov rax, [rbp - 16] ; ЛошедьБ
					pop rbx

					shr rbx, 9 ; pseudo-float emul
					mov rdx, 0
					div rbx

				push rax

				mov rax, 0 ; const value << 9
				pop rbx

				sub rax, rbx

			mov [rbp - 48], rax ; Результат = rax
			
			mov rax, [rbp - 48] ; Результат
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

			
			.4enif:

		
		.2enif:

	
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
	mov rsi, inputbuf ; buffer for inputted value

	mov rdx, 15
	xor rax, rax
	syscall
	mov rcx, rax
	call atoi
	shl rax, 9 ; pseudo-float emul
	mov [rbp - 16], rax ; ЛошедьА = rax
	
	xor rdi, rdi
	mov rsi, inputbuf ; buffer for inputted value

	mov rdx, 15
	xor rax, rax
	syscall
	mov rcx, rax
	call atoi
	shl rax, 9 ; pseudo-float emul
	mov [rbp - 24], rax ; ЛошедьБ = rax
	
	xor rdi, rdi
	mov rsi, inputbuf ; buffer for inputted value

	mov rdx, 15
	xor rax, rax
	syscall
	mov rcx, rax
	call atoi
	shl rax, 9 ; pseudo-float emul
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
	


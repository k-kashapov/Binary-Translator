global _start
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

f7938: ; def Квадрат
	push rbp ; create stack frame
	mov rbp, rsp

	; if statement
		; je
			mov rax, [rbp - 8] ; ЛошедьА
			mov rbx, rax ; save left to rbx
			mov rax, 0 ; const value
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
		mov rdi, rax
		mov rax, [rbp - 24] ; ЛошедьВ
		mov rsi, rax
		call f8991 ; call Линейная
		mov [rsp - 8], rax ; ЛошедьА = rax
		
		jmp .0enif

		.0false:

		sub rsp, 8 ; declared Дискриминант; [32; 40]
		
		mov [rsp - 32], rax ; Дискриминант = rax
		
		; if statement
			; jae
				mov rax, [rbp - 32] ; Дискриминант
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value
				cmp rbx, rax
				jae .1cmp

				xor rax, rax ; false
				jmp .1cmpEnd

				.1cmp:
				mov rax, 1 ; true

				.1cmpEnd:

			test rax, rax
			je .1false

			sub rsp, 8 ; declared Количество; [40; 48]
			
			mov rax, 2 ; const value
			mov [rsp - 40], rax ; Количество = rax
			
			mov rax, [rbp - 40] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			sub rsp, 8 ; declared Корень; [48; 56]
			
			mov [rsp - 48], rax ; Корень = rax
			
			sub rsp, 16 ; declared Корни; [56; 72]
			
			mov [rsp - 56], rax ; Корни = rax
			
			mov [rsp - 64], rax ; Корни = rax
			
			mov rax, [rbp - 56] ; Корни
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			mov rax, [rbp - 64] ; Корни
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp .1enif

			.1false:

			sub rsp, 8 ; declared Жопа; [72; 80]
			
			mov [rsp - 72], rax ; Жопа = rax
			
			mov rax, [rbp - 72] ; Жопа
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			.1enif:

		
		.0enif:

	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	

f8991: ; def Линейная
	push rbp ; create stack frame
	mov rbp, rsp

	; if statement
		; je
			mov rax, [rbp - 16] ; ЛошедьБ
			mov rbx, rax ; save left to rbx
			mov rax, 0 ; const value
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
				mov rax, 0 ; const value
				cmp rbx, rax
				je .3cmp

				xor rax, rax ; false
				jmp .3cmpEnd

				.3cmp:
				mov rax, 1 ; true

				.3cmpEnd:

			test rax, rax
			je .3false

			sub rsp, 8 ; declared Количество; [24; 32]
			
			mov rax, 100 ; const value
			mov [rsp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp .3enif

			.3false:

			mov rax, 0 ; const value
			mov [rsp - 32], rax ; Количество = rax
			
			mov rax, 1 ; const value
			mov [rsp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			sub rsp, 8 ; declared Нулёвочка; [32; 40]
			
			mov rax, 0 ; const value
			mov [rsp - 32], rax ; Нулёвочка = rax
			
			mov rax, [rbp - 32] ; Нулёвочка
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			.3enif:

		
		jmp .2enif

		.2false:

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
				mov rbx, rax ; save left to rbx
				mov rax, 0 ; const value
				cmp rbx, rax
				je .4cmp

				xor rax, rax ; false
				jmp .4cmpEnd

				.4cmp:
				mov rax, 1 ; true

				.4cmpEnd:

			test rax, rax
			je .4false

			sub rsp, 8 ; declared Жопа; [40; 48]
			
			mov [rsp - 40], rax ; Жопа = rax
			
			mov rax, [rbp - 40] ; Жопа
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp .4enif

			.4false:

			mov rax, 0 ; const value
			mov [rsp - 32], rax ; Количество = rax
			
			mov rax, 1 ; const value
			mov [rsp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			sub rsp, 8 ; declared Результат; [48; 56]
			
			mov [rsp - 48], rax ; Результат = rax
			
			mov rax, [rbp - 48] ; Результат
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			.4enif:

		
		.2enif:

	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 8 ; declared Костыль; [8; 16]
	
	sub rsp, 8 ; declared ЛошедьА; [16; 24]
	
	sub rsp, 8 ; declared ЛошедьБ; [24; 32]
	
	sub rsp, 8 ; declared ЛошедьВ; [32; 40]
	
	; in ??????????????????????????
	mov [rsp - 16], rax ; ЛошедьА = rax
	
	; in ??????????????????????????
	mov [rsp - 24], rax ; ЛошедьБ = rax
	
	; in ??????????????????????????
	mov [rsp - 32], rax ; ЛошедьВ = rax
	
	; call args
	mov rax, [rbp - 16] ; ЛошедьА
	mov rdi, rax
	mov rax, [rbp - 24] ; ЛошедьБ
	mov rsi, rax
	mov rax, [rbp - 32] ; ЛошедьВ
	mov rdx, rax
	call f7938 ; call Квадрат
	mov [rsp - 8], rax ; Костыль = rax
	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	


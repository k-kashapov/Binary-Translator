global _start
section .text
_start:
	pusha      ; push everything
	call f1058 ; call main
	popa       ; restore initial regs state
	mov rax, 0x3C
	xor rdi, rdi
	syscall

f7938: ; def Квадрат
	push rbp ; create stack frame
	mov rbp, rsp

	; if statement
		; je
			mov rax, [rbp - 8] ; ЛошедьА
			sub rsp, 8 ; declared temp left; [8; 16]
			mov rax, 0 ; const value
			cmp [rbp - 8], rax
			je 0cmp

			xor rax, rax ; false
			jmp 0cmpEnd

			0cmp:
			mov rax, 1 ; true

			0cmpEnd:

			add rsp, 8; removed variable of len 1
		test rax, rax
		jz 0false

		; call args
		mov rax, [rbp - 16] ; ЛошедьБ
		mov rdi, rax
		mov rax, [rbp - 24] ; ЛошедьВ
		mov rsi, rax
		call f8991 ; call Линейная
		mov [rsp - 8], rax ; ЛошедьА = rax
		
		jmp 0enif

		0false:

		sub rsp, 8 ; declared Дискриминант; [8; 16]
		
		mov rax, [rbp - 16] ; ЛошедьБ
		mov rax, [rbp - 16] ; ЛошедьБ
		mul
		mov rax, 4 ; const value
		mov rax, [rbp - 8] ; ЛошедьА
		mov rax, [rbp - 24] ; ЛошедьВ
		mul
		mul
		sub
		mov [rsp - 32], rax ; Дискриминант = rax
		
		; if statement
			; jae
				mov rax, [rbp - 32] ; Дискриминант
				sub rsp, 8 ; declared temp left; [8; 16]
				mov rax, 0 ; const value
				cmp [rbp - 8], rax
				jae 0cmp

				xor rax, rax ; false
				jmp 0cmpEnd

				0cmp:
				mov rax, 1 ; true

				0cmpEnd:

				add rsp, 8; removed variable of len 1
			test rax, rax
			jz 1false

			sub rsp, 8 ; declared Количество; [8; 16]
			
			mov rax, 2 ; const value
			mov [rsp - 40], rax ; Количество = rax
			
			mov rax, [rbp - 40] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			sub rsp, 8 ; declared Корень; [8; 16]
			
			mov rax, [rbp - 32] ; Дискриминант
			mov rax, 1 ; const value
			mov rax, 2 ; const value
			div
			pow
			mov [rsp - 48], rax ; Корень = rax
			
			sub rsp, 16 ; declared Корни; [8; 24]
			
			mov rax, [rbp - 48] ; Корень
			mov rax, [rbp - 16] ; ЛошедьБ
			sub
			mov rax, [rbp - 8] ; ЛошедьА
			mov rax, 2 ; const value
			mul
			div
			mov [rsp - 56], rax ; Корни = rax
			
			mov rax, 0 ; const value
			mov rax, [rbp - 48] ; Корень
			mov rax, [rbp - 16] ; ЛошедьБ
			add
			sub
			mov rax, [rbp - 8] ; ЛошедьА
			mov rax, 2 ; const value
			mul
			div
			mov [rsp - 64], rax ; Корни = rax
			
			mov rax, [rbp - 56] ; Корни
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			mov rax, [rbp - 64] ; Корни
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp 1enif

			1false:

			sub rsp, 8 ; declared Жопа; [8; 16]
			
			mov rax, 0 ; const value
			mov rax, 1 ; const value
			sub
			mov [rsp - 72], rax ; Жопа = rax
			
			mov rax, [rbp - 72] ; Жопа
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			1enif:

		
		0enif:

	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	

f8991: ; def Линейная
	push rbp ; create stack frame
	mov rbp, rsp

	; if statement
		; je
			mov rax, [rbp - 16] ; ЛошедьБ
			sub rsp, 8 ; declared temp left; [8; 16]
			mov rax, 0 ; const value
			cmp [rbp - 8], rax
			je 0cmp

			xor rax, rax ; false
			jmp 0cmpEnd

			0cmp:
			mov rax, 1 ; true

			0cmpEnd:

			add rsp, 8; removed variable of len 1
		test rax, rax
		jz 2false

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
				sub rsp, 8 ; declared temp left; [8; 16]
				mov rax, 0 ; const value
				cmp [rbp - 8], rax
				je 0cmp

				xor rax, rax ; false
				jmp 0cmpEnd

				0cmp:
				mov rax, 1 ; true

				0cmpEnd:

				add rsp, 8; removed variable of len 1
			test rax, rax
			jz 3false

			sub rsp, 8 ; declared Количество; [8; 16]
			
			mov rax, 100 ; const value
			mov [rsp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp 3enif

			3false:

			mov rax, 0 ; const value
			mov [rsp - 32], rax ; Количество = rax
			
			mov rax, 1 ; const value
			mov [rsp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			sub rsp, 8 ; declared Нулёвочка; [8; 16]
			
			mov rax, 0 ; const value
			mov [rsp - 32], rax ; Нулёвочка = rax
			
			mov rax, [rbp - 32] ; Нулёвочка
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			3enif:

		
		jmp 2enif

		2false:

		; if statement
			; je
				mov rax, [rbp - 8] ; ЛошедьК
				sub rsp, 8 ; declared temp left; [8; 16]
				mov rax, 0 ; const value
				cmp [rbp - 8], rax
				je 0cmp

				xor rax, rax ; false
				jmp 0cmpEnd

				0cmp:
				mov rax, 1 ; true

				0cmpEnd:

				add rsp, 8; removed variable of len 1
			test rax, rax
			jz 4false

			sub rsp, 8 ; declared Жопа; [8; 16]
			
			mov rax, 0 ; const value
			mov rax, 1 ; const value
			sub
			mov [rsp - 40], rax ; Жопа = rax
			
			mov rax, [rbp - 40] ; Жопа
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp 4enif

			4false:

			mov rax, 0 ; const value
			mov [rsp - 32], rax ; Количество = rax
			
			mov rax, 1 ; const value
			mov [rsp - 24], rax ; Количество = rax
			
			mov rax, [rbp - 24] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			sub rsp, 8 ; declared Результат; [8; 16]
			
			mov rax, 0 ; const value
			mov rax, [rbp - 16] ; ЛошедьБ
			mov rax, [rbp - 8] ; ЛошедьК
			div
			sub
			mov [rsp - 48], rax ; Результат = rax
			
			mov rax, [rbp - 48] ; Результат
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			4enif:

		
		2enif:

	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	sub rsp, 8 ; declared Костыль; [8; 16]
	
	sub rsp, 8 ; declared ЛошедьА; [8; 16]
	
	sub rsp, 8 ; declared ЛошедьБ; [8; 16]
	
	sub rsp, 8 ; declared ЛошедьВ; [8; 16]
	
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
	


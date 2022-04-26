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
	; if statement
		; je
			push [rbp+0] ; ЛошедьА
			push 0 ; const value
			je 0cmp
			push 0
			jmp 0cmpEnd
			0cmp:
			push 1
			0cmpEnd:
		test rax, rax
		jz 0false
		; call args
		push [rbp+1] ; ЛошедьБ
		mov rdi, rax
		push [rbp+2] ; ЛошедьВ
		mov rsi, rax
		push rbp ; create stack frame
		mov rbp, rsp
		call f8991 ; call Линейная
		pop rbp
		push rx
		pop [rbp+0] ; ЛошедьА
		
		jmp 0enif
		0false:
		push 0 ; const value
		pop [rsp+0] ; declared Дискриминант
		add rsp, 1
		
		push [rbp+1] ; ЛошедьБ
		push [rbp+1] ; ЛошедьБ
		mul
		push 4 ; const value
		push [rbp+0] ; ЛошедьА
		push [rbp+2] ; ЛошедьВ
		mul
		mul
		sub
		pop [rbp+3] ; Дискриминант
		
		; if statement
			; jae
				push [rbp+3] ; Дискриминант
				push 0 ; const value
				jae 1cmp
				push 0
				jmp 1cmpEnd
				1cmp:
				push 1
				1cmpEnd:
			test rax, rax
			jz 1false
			push 0 ; const value
			pop [rsp+1] ; declared Количество
			add rsp, 1
			
			push 2 ; const value
			pop [rbp+4] ; Количество
			
			push [rbp+4] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			push 0 ; const value
			pop [rsp+2] ; declared Корень
			add rsp, 1
			
			push [rbp+3] ; Дискриминант
			push 1 ; const value
			push 2 ; const value
			div
			pow
			pop [rbp+5] ; Корень
			
			push 0 ; const value
			pop [rsp+3] ; declared Корни
			add rsp, 2
			
			push [rbp+5] ; Корень
			push [rbp+1] ; ЛошедьБ
			sub
			push [rbp+0] ; ЛошедьА
			push 2 ; const value
			mul
			div
			pop [rbp+6] ; Корни
			
			push 0 ; const value
			push [rbp+5] ; Корень
			push [rbp+1] ; ЛошедьБ
			add
			sub
			push [rbp+0] ; ЛошедьА
			push 2 ; const value
			mul
			div
			pop [rbp+7] ; Корни
			
			push [rbp+6] ; Корни
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			push [rbp+7] ; Корни
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp 1enif
			1false:
			push 0 ; const value
			pop [rsp+5] ; declared Жопа
			add rsp, 1
			
			push 0 ; const value
			push 1 ; const value
			sub
			pop [rbp+7] ; Жопа
			
			push [rbp+7] ; Жопа
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			1enif:
		
		0enif:
	
	push 0 ; const value
	ret
	

f8991: ; def Линейная
	; if statement
		; je
			push [rbp+1] ; ЛошедьБ
			push 0 ; const value
			je 2cmp
			push 0
			jmp 2cmpEnd
			2cmp:
			push 1
			2cmpEnd:
		test rax, rax
		jz 2false
		; if statement
			; je
				push [rbp+0] ; ЛошедьК
				push 0 ; const value
				je 3cmp
				push 0
				jmp 3cmpEnd
				3cmp:
				push 1
				3cmpEnd:
			test rax, rax
			jz 3false
			push 0 ; const value
			pop [rsp+0] ; declared Количество
			add rsp, 1
			
			push 100 ; const value
			pop [rbp+2] ; Количество
			
			push [rbp+2] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp 3enif
			3false:
			push 0 ; const value
			pop [rbp+3] ; Количество
			
			push 1 ; const value
			pop [rbp+2] ; Количество
			
			push [rbp+2] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			push 0 ; const value
			pop [rsp+1] ; declared Нулёвочка
			add rsp, 1
			
			push 0 ; const value
			pop [rbp+3] ; Нулёвочка
			
			push [rbp+3] ; Нулёвочка
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			3enif:
		
		jmp 2enif
		2false:
		; if statement
			; je
				push [rbp+0] ; ЛошедьК
				push 0 ; const value
				je 4cmp
				push 0
				jmp 4cmpEnd
				4cmp:
				push 1
				4cmpEnd:
			test rax, rax
			jz 4false
			push 0 ; const value
			pop [rsp+2] ; declared Жопа
			add rsp, 1
			
			push 0 ; const value
			push 1 ; const value
			sub
			pop [rbp+4] ; Жопа
			
			push [rbp+4] ; Жопа
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			jmp 4enif
			4false:
			push 0 ; const value
			pop [rbp+3] ; Количество
			
			push 1 ; const value
			pop [rbp+2] ; Количество
			
			push [rbp+2] ; Количество
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			push 0 ; const value
			pop [rsp+3] ; declared Результат
			add rsp, 1
			
			push 0 ; const value
			push [rbp+1] ; ЛошедьБ
			push [rbp+0] ; ЛошедьК
			div
			sub
			pop [rbp+5] ; Результат
			
			push [rbp+5] ; Результат
			; out !!!!!!!!!!!!!!!!!!!!!!!!
			
			4enif:
		
		2enif:
	
	push 0 ; const value
	ret
	

f1058: ; def main
	push 0 ; const value
	pop [rsp+0] ; declared Костыль
	add rsp, 1
	
	push 0 ; const value
	pop [rsp+1] ; declared ЛошедьА
	add rsp, 1
	
	push 0 ; const value
	pop [rsp+2] ; declared ЛошедьБ
	add rsp, 1
	
	push 0 ; const value
	pop [rsp+3] ; declared ЛошедьВ
	add rsp, 1
	
	; in ?????????????????????????
	pop [rbp+1] ; ЛошедьА
	
	; in ?????????????????????????
	pop [rbp+2] ; ЛошедьБ
	
	; in ?????????????????????????
	pop [rbp+3] ; ЛошедьВ
	
	; call args
	push [rbp+1] ; ЛошедьА
	mov rdi, rax
	push [rbp+2] ; ЛошедьБ
	mov rsi, rax
	push [rbp+3] ; ЛошедьВ
	mov rdx, rax
	push rbp ; create stack frame
	mov rbp, rsp
	call f7938 ; call Квадрат
	pop rbp
	push rx
	pop [rbp+0] ; Костыль
	
	push 0 ; const value
	ret
	


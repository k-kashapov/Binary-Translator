global _start
section .text
f7938: ; def Квадрат
	; if statement
		; je
			push [rbp+0] ; ЛошедьА
			push 0 ; const value
			je :0cmp
			push 0
			jmp :0cmpEnd
			0cmp:
			push 1
			0cmpEnd:
		push 0
		je :0false
		push [rbp+1] ; ЛошедьБ
		mov ArgumentRegs[curr_register] "rax"
		push [rbp+2] ; ЛошедьВ
		mov ArgumentRegs[curr_register] "rax"
		push rbp ; create stack frame
		mov "rbp" "rsp"
		call :f8991 ; call Линейная
		pop "rbp"
		add rsp, 0 ; pushed args
		push rx
		pop [rbp+0] ; ЛошедьА
		
		jmp :0enif
		0false:
		push 0 ; const value
		pop [rsp+0] ; declared Дискриминант
		add "rsp" len
		
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
				jae :1cmp
				push 0
				jmp :1cmpEnd
				1cmp:
				push 1
				1cmpEnd:
			push 0
			je :1false
			push 0 ; const value
			pop [rsp+1] ; declared Количество
			add "rsp" len
			
			push 2 ; const value
			pop [rbp+4] ; Количество
			
			push [rbp+4] ; Количество
			out ; Количество
			pop tx ; to trash
			
			push 0 ; const value
			pop [rsp+2] ; declared Корень
			add "rsp" len
			
			push [rbp+3] ; Дискриминант
			push 1 ; const value
			push 2 ; const value
			div
			pow
			pop [rbp+5] ; Корень
			
			push 0 ; const value
			pop [rsp+3] ; declared Корни
			add "rsp" len
			
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
			out ; Корни
			pop tx ; to trash
			
			push [rbp+7] ; Корни
			out ; Корни
			pop tx ; to trash
			
			jmp :1enif
			1false:
			push 0 ; const value
			pop [rsp+5] ; declared Жопа
			add "rsp" len
			
			push 0 ; const value
			push 1 ; const value
			sub
			pop [rbp+7] ; Жопа
			
			push [rbp+7] ; Жопа
			out ; Жопа
			pop tx ; to trash
			
			1enif:
		
		0enif:
	
	push 0 ; const value
	pop rax

	ret
	

f8991: ; def Линейная
	; if statement
		; je
			push [rbp+1] ; ЛошедьБ
			push 0 ; const value
			je :2cmp
			push 0
			jmp :2cmpEnd
			2cmp:
			push 1
			2cmpEnd:
		push 0
		je :2false
		; if statement
			; je
				push [rbp+0] ; ЛошедьК
				push 0 ; const value
				je :3cmp
				push 0
				jmp :3cmpEnd
				3cmp:
				push 1
				3cmpEnd:
			push 0
			je :3false
			push 0 ; const value
			pop [rsp+0] ; declared Количество
			add "rsp" len
			
			push 100 ; const value
			pop [rbp+2] ; Количество
			
			push [rbp+2] ; Количество
			out ; Количество
			pop tx ; to trash
			
			jmp :3enif
			3false:
			push 0 ; const value
			pop [rbp+3] ; Количество
			
			push 1 ; const value
			pop [rbp+2] ; Количество
			
			push [rbp+2] ; Количество
			out ; Количество
			pop tx ; to trash
			
			push 0 ; const value
			pop [rsp+1] ; declared Нулёвочка
			add "rsp" len
			
			push 0 ; const value
			pop [rbp+3] ; Нулёвочка
			
			push [rbp+3] ; Нулёвочка
			out ; Нулёвочка
			pop tx ; to trash
			
			3enif:
		
		jmp :2enif
		2false:
		; if statement
			; je
				push [rbp+0] ; ЛошедьК
				push 0 ; const value
				je :4cmp
				push 0
				jmp :4cmpEnd
				4cmp:
				push 1
				4cmpEnd:
			push 0
			je :4false
			push 0 ; const value
			pop [rsp+2] ; declared Жопа
			add "rsp" len
			
			push 0 ; const value
			push 1 ; const value
			sub
			pop [rbp+4] ; Жопа
			
			push [rbp+4] ; Жопа
			out ; Жопа
			pop tx ; to trash
			
			jmp :4enif
			4false:
			push 0 ; const value
			pop [rbp+3] ; Количество
			
			push 1 ; const value
			pop [rbp+2] ; Количество
			
			push [rbp+2] ; Количество
			out ; Количество
			pop tx ; to trash
			
			push 0 ; const value
			pop [rsp+3] ; declared Результат
			add "rsp" len
			
			push 0 ; const value
			push [rbp+1] ; ЛошедьБ
			push [rbp+0] ; ЛошедьК
			div
			sub
			pop [rbp+5] ; Результат
			
			push [rbp+5] ; Результат
			out ; Результат
			pop tx ; to trash
			
			4enif:
		
		2enif:
	
	push 0 ; const value
	pop rax

	ret
	

_start: ; def main
f1058: ; def main
	push 0 ; const value
	pop [rsp+0] ; declared Костыль
	add "rsp" len
	
	push 0 ; const value
	pop [rsp+1] ; declared ЛошедьА
	add "rsp" len
	
	push 0 ; const value
	pop [rsp+2] ; declared ЛошедьБ
	add "rsp" len
	
	push 0 ; const value
	pop [rsp+3] ; declared ЛошедьВ
	add "rsp" len
	
	in
	pop [rbp+1] ; ЛошедьА
	
	in
	pop [rbp+2] ; ЛошедьБ
	
	in
	pop [rbp+3] ; ЛошедьВ
	
	push [rbp+1] ; ЛошедьА
	mov ArgumentRegs[curr_register] "rax"
	push [rbp+2] ; ЛошедьБ
	mov ArgumentRegs[curr_register] "rax"
	push [rbp+3] ; ЛошедьВ
	mov ArgumentRegs[curr_register] "rax"
	push rbp ; create stack frame
	mov "rbp" "rsp"
	call :f7938 ; call Квадрат
	pop "rbp"
	add rsp, 0 ; pushed args
	push rx
	pop [rbp+0] ; Костыль
	
	push 0 ; const value
	pop rax

	ret
	


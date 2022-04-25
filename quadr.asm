push 0
pop mx ; available memory offset
push 0
pop fx ; free memory offset
push 0
pop rx ; call result

call :f1058 ; main
hlt

f7938: ; def Квадрат
	; if statement
		; je
			push [mx+0] ; ЛошедьА
			push 0 ; const value
			je :0cmp
			push 0
			jmp :0cmpEnd
			0cmp:
			push 1
			0cmpEnd:
		push 0
		je :0false
		push fx ; save free
		push mx ; save mem
		push fx ; save freeOffest to stack
		push [mx+1] ; ЛошедьБ
		pop [fx]
		; moving ptr fx
			push 1
			push fx
			add
			pop fx ; ptr fx moved
		; ptr fx moved
		push [mx+2] ; ЛошедьВ
		pop [fx]
		; moving ptr fx
			push 1
			push fx
			add
			pop fx ; ptr fx moved
		; ptr fx moved
		pop mx ; pop freeOffest from stack
		call :f8991 ; call Линейная
		pop mx
		pop fx
		push rx
		pop [mx+0] ; ЛошедьА
		
		jmp :0enif
		0false:
		push 0 ; const value
		pop [fx+2] ; declared Дискриминант
		; moving ptr fx
			push 1
			push fx
			add
			pop fx ; ptr fx moved
		; ptr fx moved
		
		push [mx+1] ; ЛошедьБ
		push [mx+1] ; ЛошедьБ
		mul
		push 4 ; const value
		push [mx+0] ; ЛошедьА
		push [mx+2] ; ЛошедьВ
		mul
		mul
		sub
		pop [mx+3] ; Дискриминант
		
		; if statement
			; jae
				push [mx+3] ; Дискриминант
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
			pop [fx+3] ; declared Количество
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push 2 ; const value
			pop [mx+4] ; Количество
			
			push [mx+4] ; Количество
			out ; Количество
			pop tx ; to trash
			
			push 0 ; const value
			pop [fx+4] ; declared Корень
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push [mx+3] ; Дискриминант
			push 1 ; const value
			push 2 ; const value
			div
			pow
			pop [mx+5] ; Корень
			
			push 0 ; const value
			pop [fx+5] ; declared Корни
			; moving ptr fx
				push 2
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push [mx+5] ; Корень
			push [mx+1] ; ЛошедьБ
			sub
			push [mx+0] ; ЛошедьА
			push 2 ; const value
			mul
			div
			pop [mx+6] ; Корни
			
			push 0 ; const value
			push [mx+5] ; Корень
			push [mx+1] ; ЛошедьБ
			add
			sub
			push [mx+0] ; ЛошедьА
			push 2 ; const value
			mul
			div
			pop [mx+7] ; Корни
			
			push [mx+6] ; Корни
			out ; Корни
			pop tx ; to trash
			
			push [mx+7] ; Корни
			out ; Корни
			pop tx ; to trash
			
			jmp :1enif
			1false:
			push 0 ; const value
			pop [fx+7] ; declared Жопа
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push 0 ; const value
			push 1 ; const value
			sub
			pop [mx+7] ; Жопа
			
			push [mx+7] ; Жопа
			out ; Жопа
			pop tx ; to trash
			
			1enif:
		
		0enif:
	
	push 0 ; const value
	pop rx

	ret
	

f8991: ; def Линейная
	; if statement
		; je
			push [mx+1] ; ЛошедьБ
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
				push [mx+0] ; ЛошедьК
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
			pop [fx+0] ; declared Количество
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push 100 ; const value
			pop [mx+2] ; Количество
			
			push [mx+2] ; Количество
			out ; Количество
			pop tx ; to trash
			
			jmp :3enif
			3false:
			push 0 ; const value
			pop [mx+3] ; Количество
			
			push 1 ; const value
			pop [mx+2] ; Количество
			
			push [mx+2] ; Количество
			out ; Количество
			pop tx ; to trash
			
			push 0 ; const value
			pop [fx+1] ; declared Нулёвочка
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push 0 ; const value
			pop [mx+3] ; Нулёвочка
			
			push [mx+3] ; Нулёвочка
			out ; Нулёвочка
			pop tx ; to trash
			
			3enif:
		
		jmp :2enif
		2false:
		; if statement
			; je
				push [mx+0] ; ЛошедьК
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
			pop [fx+2] ; declared Жопа
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push 0 ; const value
			push 1 ; const value
			sub
			pop [mx+4] ; Жопа
			
			push [mx+4] ; Жопа
			out ; Жопа
			pop tx ; to trash
			
			jmp :4enif
			4false:
			push 0 ; const value
			pop [mx+3] ; Количество
			
			push 1 ; const value
			pop [mx+2] ; Количество
			
			push [mx+2] ; Количество
			out ; Количество
			pop tx ; to trash
			
			push 0 ; const value
			pop [fx+3] ; declared Результат
			; moving ptr fx
				push 1
				push fx
				add
				pop fx ; ptr fx moved
			; ptr fx moved
			
			push 0 ; const value
			push [mx+1] ; ЛошедьБ
			push [mx+0] ; ЛошедьК
			div
			sub
			pop [mx+5] ; Результат
			
			push [mx+5] ; Результат
			out ; Результат
			pop tx ; to trash
			
			4enif:
		
		2enif:
	
	push 0 ; const value
	pop rx

	ret
	

f1058: ; def main
	push 0 ; const value
	pop [fx+0] ; declared Костыль
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	push 0 ; const value
	pop [fx+1] ; declared ЛошедьА
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	push 0 ; const value
	pop [fx+2] ; declared ЛошедьБ
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	push 0 ; const value
	pop [fx+3] ; declared ЛошедьВ
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	in
	pop [mx+1] ; ЛошедьА
	
	in
	pop [mx+2] ; ЛошедьБ
	
	in
	pop [mx+3] ; ЛошедьВ
	
	push fx ; save free
	push mx ; save mem
	push fx ; save freeOffest to stack
	push [mx+1] ; ЛошедьА
	pop [fx]
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	push [mx+2] ; ЛошедьБ
	pop [fx]
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	push [mx+3] ; ЛошедьВ
	pop [fx]
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	pop mx ; pop freeOffest from stack
	call :f7938 ; call Квадрат
	pop mx
	pop fx
	push rx
	pop [mx+0] ; Костыль
	
	push 0 ; const value
	pop rx

	ret
	


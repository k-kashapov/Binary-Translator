push 0
pop mx ; available memory offset
push 0
pop fx ; free memory offset
push 0
pop rx ; call result

call :f1058 ; main
hlt

f11528: ; def Факториал
	push 0 ; const value
	pop [fx+0] ; declared Результат
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	; if statement
		; ja
			push [mx+0] ; Итератор
			push 1 ; const value
			ja :0cmp
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
		push [mx+0] ; Итератор
		push 1 ; const value
		sub
		pop [fx]
		; moving ptr fx
			push 1
			push fx
			add
			pop fx ; ptr fx moved
		; ptr fx moved
		pop mx ; pop freeOffest from stack
		call :f11528 ; call Факториал
		pop mx
		pop fx
		push rx
		pop [mx+1] ; Результат
		
		jmp :0enif
		0false:
		push 1 ; const value
		pop [mx+1] ; Результат
		
		0enif:
	
	push [mx+1] ; Результат
	push [mx+0] ; Итератор
	mul
	pop rx

	ret
	

f1058: ; def main
	push 0 ; const value
	pop [fx+0] ; declared Резалт
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	push 0 ; const value
	pop [fx+1] ; declared База
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	push 0 ; const value
	pop [fx+2] ; declared Повторы
	; moving ptr fx
		push 1
		push fx
		add
		pop fx ; ptr fx moved
	; ptr fx moved
	
	push 12 ; const value
	pop [mx+1] ; База
	
	push 100000 ; const value
	pop [mx+2] ; Повторы
	
	; while
		0while:
		; ja
			push [mx+2] ; Повторы
			push 0 ; const value
			ja :1cmp
			push 0
			jmp :1cmpEnd
			1cmp:
			push 1
			1cmpEnd:
		push 0
		je :0whileEnd
		push [mx+2] ; Повторы
		push 1 ; const value
		sub
		pop [mx+2] ; Повторы
		
		push fx ; save free
		push mx ; save mem
		push fx ; save freeOffest to stack
		push [mx+1] ; База
		pop [fx]
		; moving ptr fx
			push 1
			push fx
			add
			pop fx ; ptr fx moved
		; ptr fx moved
		pop mx ; pop freeOffest from stack
		call :f11528 ; call Факториал
		pop mx
		pop fx
		push rx
		pop [mx+0] ; Резалт
		
		jmp :0while
		0whileEnd:
	
	push [mx+0] ; Резалт
	out ; Резалт
	pop tx ; to trash
	
	push [mx+0] ; Резалт
	pop rx

	ret
	


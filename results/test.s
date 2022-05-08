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

	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	

f8991: ; def Линейная
	push rbp ; create stack frame
	mov rbp, rsp

	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	

f1058: ; def main
	push rbp ; create stack frame
	mov rbp, rsp

	mov rax, 0 ; const value
	mov [rsp - 8], rax ; declared Костыль
	
	mov rax, 0 ; const value
	mov [rsp - 16], rax ; declared ЛошедьА
	
	mov rax, 0 ; const value
	mov [rsp - 24], rax ; declared ЛошедьБ
	
	mov rax, 0 ; const value
	mov [rsp - 32], rax ; declared ЛошедьВ
	
	; in ??????????????????????????
	mov [rsp - 1], rax ; ЛошедьА = rax
	
	; in ??????????????????????????
	mov [rsp - 2], rax ; ЛошедьБ = rax
	
	; in ??????????????????????????
	mov [rsp - 3], rax ; ЛошедьВ = rax
	
	; call args
	mov rax, [rsp - 8] ; ЛошедьА
	mov rdi, rax
	mov rax, [rsp - 16] ; ЛошедьБ
	mov rsi, rax
	mov rax, [rsp - 24] ; ЛошедьВ
	mov rdx, rax
	call f7938 ; call Квадрат
	mov [rsp - 0], rax ; Костыль = rax
	
	mov rax, 0 ; const value
	pop rbp ; stack frame return

	ret
	


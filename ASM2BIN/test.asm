
section	.text
	global _start       
_start:                
    label:
sub rsp, 8
sub rsp, 16
sub rsp, 258
sub rsp, 512
sub rsp, 666
mov [rsp - 8], rax
mov [rsp - 16], rax
mov [rsp - 258], rax
mov [rsp - 512], rax
mov [rsp - 666], rax
call label
mov rsp, rbp
pop rbp
ret
push rbp
mov rbp, rsp
xor rdi, rdi
mov rsi, 00
lea rsi, [inputbuf]
mov rdx, 15
xor rax, rax
syscall
mov rcx, rax
sal rax, 9
sar rax, 9
mov r12, rsp
test rax, rax
.while:
je .whileEnd
mov rsp, r12
jmp .while
push rax
pop rbx
add rax, rbx
sub rax, rbx
.whileEnd:

section .bss
inputbuf: resq 0


section	.text
	global _start       
_start:                
    label:

sub rsp, 8
sub rsp, 16
sub rsp, 258
sub rsp, 512
sub rsp, 666

add rsp, 8
add rsp, 666

mov [rsp - 8], rax
mov [rsp - 16], rax
mov [rsp - 258], rax
mov [rsp - 512], rax
mov [rsp - 666], rax

mov [rbp - 0x414522], rax

mov rax, 0x2
mov rax, 0x214532

mov rax, [rsp - 1]
mov rax, [rsp - 666]

mov rax, [rbp - 1]
mov rax, [rbp - 666]

not rax

mov rbx, rax
cmp rbx, rax

je  jojo
jge jojo
jle jojo
jg  jojo
jl  jojo
jne jojo

jojo:

call label
call 0x424569

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

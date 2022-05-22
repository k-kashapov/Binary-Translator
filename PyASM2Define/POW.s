72 000000AA 4885C0                  			test rax, rax
73 000000AD 7452                    			jz .DontPow
74 000000AF 4883F801                			cmp rax, 1
75 000000B3 744C                    			je .DontPow
76 000000B5 50                      			push rax
77                                  
78 000000B6 B800020000              			mov rax, 512 ; const value << 9
79 000000BB 4883F801                			cmp rax, 1
80 000000BF 743F                    			je .DontPowButPop
81 000000C1 50                      			push rax
82                                  
83 000000C2 DF0424                  			fild  WORD [rsp]            ; load base onto FPU stack
84 000000C5 DA3425[00000000]        			fidiv DWORD [const_for_pow] ; convert from pseudo-float
85                                  
86 000000CC DF442408                			fild  WORD [rsp + 8]      ; load power onto FPU stack
87 000000D0 DA3425[00000000]        			fidiv DWORD [const_for_pow] ; convert from pseudo-float
88                                  
89 000000D7 D9F1                    			fyl2x ; power * log_2_(base)
90                                  
91                                  			; value between -1 and 1 is required by pow of 2 command
92 000000D9 DB5424F8                			fist DWORD [rsp - 8] ; cast to int
93 000000DD DB4424F8                			fild DWORD [rsp - 8] ;
94 000000E1 DEE9                    			fsub      ; fit into [-1; 1]
95                                  
96 000000E3 D9F0                    			f2xm1 ; 2^(power * log_2_(base)) - 1 = base^power
97                                  
98 000000E5 D9E8                    			fld1   ; push 1
99 000000E7 DEC1                    			fadd   ; add 1 to the result
100                                  
101 000000E9 DB4424F8                			fild DWORD [rsp - 8] ; load casted value
102 000000ED D9C9                    			fxch   ; exchange st(0) <-> st(1)
103 000000EF D9FD                    			fscale ; multiply by remaining power of 2
104 000000F1 DA0C25[00000000]        			fimul DWORD [const_for_pow] ; to pseudo-float
105 000000F8 DB5C2408                			fistp DWORD [rsp + 8]      ; save pow value to stack
106                                  
107 000000FC 4883C408                			add rsp, 8
108                                  			.DontPowButPop:
109 00000100 58                      			pop rax
110                                  			.DontPow:
//      MNEMO                                     OPCODE     LEN  ARG_LEN   ARG
#define SUB_RSP_1_BYTE(arg_)       (INSTRUCTION { 0xEC8348,    3,       1, arg_ })
#define SUB_RSP_4_BYTE(arg_)       (INSTRUCTION { 0xEC8148,    3,       4, arg_ })
#define ADD_RSP_1_BYTE(arg_)       (INSTRUCTION { 0xC48348,    3,       1, arg_ })
#define ADD_RSP_4_BYTE(arg_)       (INSTRUCTION { 0xC48148,    3,       4, arg_ })
#define RET_1_BYTE                 (INSTRUCTION { 0xC3,        1,       0,    0 })

#define MOV_RSP_RBP                (INSTRUCTION { 0xEC8948,    3,       0,    0 })
#define POP_RBP                    (INSTRUCTION { 0x5d,        1,       0,    0 })

// Call expects relative addr:
//   ... E8 ** ** ** **
//      ^   <---ARG--->
//     RIP

// ARG = offset Destination - (RIP + 1 + 4)
//                                   ^   ^
//                           OPCODE--^   ^--ARG

#define CALL_NEAR_4_BYTE(arg_)     (INSTRUCTION { 0xE8,        1,       4, arg_ })

#define PUSH_RBP                   (INSTRUCTION { 0x55,        1,       0,    0 })
#define MOV_RBP_RSP                (INSTRUCTION { 0xE58948,    3,       0,    0 })

#define XOR_RDI_RDI                (INSTRUCTION { 0xFF3148,    3,       0,    0 })
#define XOR_RAX_RAX                (INSTRUCTION { 0xC03148,    3,       0,    0 })

#define LEA_RSI_4_BYTE(arg_)       (INSTRUCTION { 0x25348D48,  4,       4, arg_ })

#define MOV_RDX_4_BYTE(arg_)       (INSTRUCTION { 0xBA,        1,       4, arg_ })

#define SYSCALL                    (INSTRUCTION { 0x050F,      2,       0,    0 })

#define MOV_RCX_RAX                (INSTRUCTION { 0xC18948,    3,       0,    0 })
#define SHL_RAX_1_BYTE(arg_)       (INSTRUCTION { 0xE0C148,    3,       1, arg_ })
#define SAR_RAX_1_BYTE(arg_)       (INSTRUCTION { 0xF8C148,    3,       1, arg_ })

#define SAR_RBX_1_BYTE(arg_)       (INSTRUCTION { 0xFBC148,    3,       1, arg_ })

#define MOV_R12_RSP                (INSTRUCTION { 0xE48949,    3,       0,    0 })
#define MOV_RSP_R12                (INSTRUCTION { 0xE4894C,    3,       0,    0 })

#define TEST_RAX_RAX               (INSTRUCTION { 0xC08548,    3,       0,    0 })

#define JE_1_BYTE(arg_)            (INSTRUCTION { 0x74,        1,       1, arg_ })
#define JGE_1_BYTE(arg_)           (INSTRUCTION { 0x7D,        1,       1, arg_ })
#define JLE_1_BYTE(arg_)           (INSTRUCTION { 0x7E,        1,       1, arg_ })
#define JG_1_BYTE(arg_)            (INSTRUCTION { 0x7F,        1,       1, arg_ })
#define JL_1_BYTE(arg_)            (INSTRUCTION { 0x7C,        1,       1, arg_ })
#define JNE_1_BYTE(arg_)           (INSTRUCTION { 0x75,        1,       1, arg_ })

#define JMP_4_BYTE(arg_)           (INSTRUCTION { 0xE9,        1,       4, arg_ })

#define PUSH_RAX                   (INSTRUCTION { 0x50,        1,       0,    0 })
#define POP_RBX                    (INSTRUCTION { 0x5B,        1,       0,    0 })

#define ADD_RAX_RBX                (INSTRUCTION { 0xD80148,    3,       0,    0 })
#define SUB_RAX_RBX                (INSTRUCTION { 0xD82948,    3,       0,    0 })

// mov [rsp + arg], rax
#define MOV_TO_STACK_2_BYTE(arg_)  (INSTRUCTION { 0x24848948,  4,       2, arg_ })

// mov rax, arg
#define MOV_RAX_4_BYTE(arg_)       (INSTRUCTION { 0xB8,        1,       4, arg_ })

// mov [rbp + arg], rax
#define MOV_RBP_4_BYTE_RAX(arg_)   (INSTRUCTION { 0x858948,    3,       4, arg_ })

// mov rax, [rsp + arg]
#define MOV_RAX_STACK_2_BYTE(arg_) (INSTRUCTION { 0x24848B48,  4,       2, arg_ })

// mov rax, [rbp + arg]
#define MOV_RAX_RBP_4_BYTE(arg_)   (INSTRUCTION { 0x858B48,    3,       4, arg_ })

#define NOT_RAX                    (INSTRUCTION { 0xD0F748,    3,       0,    0 })

#define MOV_RBX_RAX                (INSTRUCTION { 0xC38948,    3,       0,    0 })
#define CMP_RBX_RAX                (INSTRUCTION { 0xC33948,    3,       0,    0 })

#define CQO_1_BYTE                 (INSTRUCTION { 0x9948,      2,       0,    0 })
#define IMUL_RBX                   (INSTRUCTION { 0xEBF748,    3,       0,    0 })
#define IDIV_RBX                   (INSTRUCTION { 0xFBF748,    3,       0,    0 })

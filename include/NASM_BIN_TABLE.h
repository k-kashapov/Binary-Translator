//      MNEMO                                    OPCODE     LEN  ARG_LEN   ARG
#define SUB_RSP_1_BYTE(arg_)      (INSTRUCTION { 0xEC8348,    3,       1, arg_ })
#define SUB_RSP_4_BYTE(arg_)      (INSTRUCTION { 0xEC8148,    3,       4, arg_ })
#define ADD_RSP_1_BYTE(arg_)      (INSTRUCTION { 0xC48348,    3,       1, arg_ })
#define ADD_RSP_4_BYTE(arg_)      (INSTRUCTION { 0xC48148,    3,       4, arg_ })
#define RET_1_BYTE                (INSTRUCTION { 0xC3,        1,       0,    0 })

#define MOV_TO_STACK_4_BYTE(arg_) (INSTRUCTION { 0x24848948,  4,       4, arg_ })

#define MOV_RSP_RBP               (INSTRUCTION { 0xEC8948,    3,       0,    0 })
#define POP_RBP                   (INSTRUCTION { 0x5d,        1,       0,    0 })

#define CALL_NEAR_4_BYTE(arg_)    (INSTRUCTION { 0xE8,        1,       4, arg_ })

#define PUSH_RBP                  (INSTRUCTION { 0x55,        1,       0,    0 })
#define MOV_RBP_RSP               (INSTRUCTION { 0xE58948,    3,       0,    0 })

#define XOR_RDI_RDI               (INSTRUCTION { 0xFF3148,    3,       0,    0 })
#define XOR_RAX_RAX               (INSTRUCTION { 0xC03148,    3,       0,    0 })

#define LEA_RSI_4_BYTE(arg_)      (INSTRUCTION { 0x25348D48,  4,       4, arg_ })

#define MOV_RDX_4_BYTE(arg_)      (INSTRUCTION { 0xBA,        1,       4, arg_ })

#define SYSCALL                   (INSTRUCTION { 0x050F,      2,       0,    0 })

#define MOV_RCX_RAX               (INSTRUCTION { 0xC18948,    3,       0,    0 })
#define SHL_RAX_1_BYTE(arg_)      (INSTRUCTION { 0xE0C148,    4,       1, arg_ })
#define SAR_RAX_1_BYTE(arg_)      (INSTRUCTION { 0xF8C148,    4,       1, arg_ })

#define MOV_R12_RSP               (INSTRUCTION { 0xE48949,    3,       0,    0 })

#define TEST_RAX_RAX              (INSTRUCTION { 0xC08548,    3,       0,    0 })

#define JE_1_BYTE(arg_)           (INSTRUCTION { 0x74,        1,       1, arg_ })

#define MOV_RSP_R12               (INSTRUCTION { 0xE4894C,    3,       0,    0 })

#define JMP_4_BYTE(arg_)          (INSTRUCTION { 0xE9,        1,       4, arg_ })

#define PUSH_RAX                  (INSTRUCTION { 0x50,        1,       0,    0 })
#define POP_RBX                   (INSTRUCTION { 0x5B,        1,       0,    0 })

#define ADD_RAX_RBX               (INSTRUCTION { 0xD80148,    3,       0,    0 })
#define SUB_RAX_RBX               (INSTRUCTION { 0xD82948,    3,       0,    0 })

/*
#define BIN (48 83 ec 08          , "sub    rsp,0x8")
#define BIN (48 83 ec 10          , "sub    rsp,0x10")
#define BIN (48 81 ec 02 01 00 00 , "sub    rsp,0x102")
#define BIN (48 81 ec 00 02 00 00 , "sub    rsp,0x200")
#define BIN (48 81 ec 9a 02 00 00 , "sub    rsp,0x29a")

#define BIN (48 83 c4 08          , "add    rsp,0x8")
#define BIN (48 81 c4 9a 02 00 00 , "add    rsp,0x29a")

#define BIN (48 89 44 24 f8       , "mov    QWORD PTR [rsp-0x8],rax")
#define BIN (48 89 44 24 f0       , "mov    QWORD PTR [rsp-0x10],rax")
#define BIN (48 89 84 24 fe fe ff ff , "mov    QWORD PTR [rsp-0x102],rax")
#define BIN (48 89 84 24 00 fe ff ff , "mov    QWORD PTR [rsp-0x200],rax")
#define BIN (48 89 84 24 66 fd ff ff , "mov    QWORD PTR [rsp-0x29a],rax")
#define BIN (e8 b1 ff ff ff       , "call   0 <_start>")
#define BIN (48 89 ec             , "mov    rsp,rbp")
#define BIN (5d                   , "pop    rbp")
#define BIN (c3                   , "ret    ")
#define BIN (55                   , "push   rbp")
#define BIN (48 89 e5             , "mov    rbp,rsp")
#define BIN (48 31 ff             , "xor    rdi,rdi")
#define BIN (be 00 00 00 00       , "mov    esi,0x0")
#define BIN (48 8d 34 25 00 00 00 00 , "lea    rsi,ds:0x0")
#define BIN (ba 0f 00 00 00       , "mov    edx,0xf")
#define BIN (48 31 c0             , "xor    rax,rax")
#define BIN (0f 05                , "syscall ")
#define BIN (48 89 c1             , "mov    rcx,rax")
#define BIN (48 c1 e0 09          , "shl    rax,0x9")
#define BIN (48 c1 f8 09          , "sar    rax,0x9")
#define BIN (49 89 e4             , "mov    r12,rsp")
#define BIN (48 85 c0             , "test   rax,rax")
#define BIN (74 0d                , "je     92 <label.whileEnd>")
#define BIN (4c 89 e4             , "mov    rsp,r12")
#define BIN (eb f9                , "jmp    83 <label.while>")
#define BIN (50                   , "push   rax")
#define BIN (5b                   , "pop    rbx")
#define BIN (48 01 d8             , "add    rax,rbx")
#define BIN (48 29 d8             , "sub    rax,rbx")
*/

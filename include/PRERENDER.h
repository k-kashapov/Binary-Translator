// Compilation of precompiled bytecodes for my own std lib

// Special thanks to Varnike and EnikAs on github for helping with
// BYTECODE and ELF generation

static const unsigned char PUSH_EVERYTING[] =
    {
        0x53,        // push rbx
        0x55,        // push rbp
        0x41, 0x54,  // push r12
        0x41, 0x55,  // push r13
        0x41, 0x56,  // push r14
        0x41, 0x57   // push r15
    };

static const unsigned char POP_EVERYTING[] =
    {
        0x41, 0x5F, // push r15
        0x41, 0x5E, // push r14
        0x41, 0x5D, // push r13
        0x41, 0x5C, // push r12
        0x5D,       // push rbp
        0x5B        // push rbx
    };

static const unsigned char EXIT[] =
    {
        0x48, 0x89, 0xc7,             // mov rdi, rax        
        0xB8, 0x3C, 0x00, 0x00, 0x00, // mov eax, 0x3c
        0x0F, 0x05                    // syscall
    };


static const unsigned char IN_CODE[] =
    {
        // <IN>
            // <READ>
                0x48, 0x31, 0xff,               // xor rdi, rdi
                0x48, 0x83, 0xec, 0x08,         // sub rsp, 8
                0x48, 0x89, 0xe6,               // mov rsi, rsp
                0xba, 0x07, 0x00, 0x00, 0x00,   // mov edx, 7
                0x48, 0x31, 0xc0,               // xor rax, rax
                0x0f, 0x05,                     // syscall
                0x48, 0x89, 0xe6,               // mov rsi, rsp
                0x48, 0x89, 0xc1,               // mov rcx, rax
                0xe8, 0x09, 0x00, 0x00, 0x00,   // call ATOI
                0x48, 0x83, 0xc4, 0x08,         // add rsp, 8
                0x48, 0xc1, 0xe0, 0x09,         // shl rax, 9
                0xc3,                           // ret
            // <ATOI>
                0x48, 0x31, 0xc0,        // xor rax, rax
                0x48, 0x31, 0xdb,        // xor rbx, rbx
                0x48, 0xff, 0xc9,        // dec rcx
                0x74, 0x21,              // je .End
                0x80, 0x3e, 0x2d,        // cmp byte [rsi], 0x2d
                0x75, 0x06,              // jne .Loop
                0x48, 0xff, 0xc6,        // inc rsi
                0x48, 0xff, 0xc9,        // dec rcx
            //.Loop:
                0x8a, 0x1e,                     // mov bl, byte [rsi]
                0x48, 0x83, 0xeb, 0x30,         // sub rbx, 0x30
                0x48, 0xff, 0xc6,               // inc rsi
                0xba, 0x0a, 0x00, 0x00, 0x00,   // mov edx, 0xa
                0x48, 0xf7, 0xe2,               // mul rdx
                0x48, 0x01, 0xd8,               // add rax, rbx
                0xe2, 0xea,                     // loop .Loop
            //.End:
                0x80, 0x7c, 0x24, 0x08, 0x2d,   // cmp byte [rsp + 8], 0x2d
                0x75, 0x03,                     // jne .Ret
                0x48, 0xf7, 0xd8,               // neg rax
            //.Ret:
                0xc3                            // ret
            // </ATOI>
        // </IN>
    };

static const unsigned char OUT_CODE[] =
    {
        // <OUT>
            // <BASE>
                0x55,                           // push rbp
                0x48, 0x89, 0xe5,               // mov rbp, rsp
                0x48, 0x83, 0xec, 0x20,         // sub rsp, 0x20
                0x48, 0x89, 0xe7,               // mov rdi, rsp
                0xc6, 0x07, 0x0a,               // mov byte [rdi], 0xa
                0x48, 0xff, 0xc7,               // inc rdi
                0x66, 0xc7, 0x07, 0x3e, 0x3e,   // mov word [rdi], 0x3e3e
                0x48, 0x83, 0xc7, 0x02,         // add rdi, 2
                0xc6, 0x07, 0x20,               // mov byte [rdi], 0x20
                0x48, 0xff, 0xc7,               // inc rdi
                0x48, 0x83, 0xf8, 0x00,         // cmp rax, 0
                0x7d, 0x09,                     // jge NotNegative
                0xc6, 0x07, 0x2d,               // mov byte [rdi], 0x2d
                0x48, 0xff, 0xc7,               // inc rdi
                0x48, 0xf7, 0xd8,               // neg rax
                0x50,                           // push rax
            //.NotNegative:
                0x48, 0xc1, 0xe8, 0x09,             // shr rax, 9
                0x48, 0x89, 0xc2,                   // mov rdx, rax
                0x41, 0xba, 0x0a, 0x00, 0x00, 0x00,        // mov r10d, 0xa
                0xe8, 0x4f, 0x00, 0x00, 0x00,        // call CntBytes
                0xe8, 0x6b, 0x00, 0x00, 0x00,        // call itoa10
                0x58,        // pop rax
                0x4c, 0x01, 0xc7,        // add rdi, r8
                0xc6, 0x07, 0x2e,        // mov byte [rdi], 0x2e
                0x48, 0xff, 0xc7,        // inc rdi
                0x48, 0x25, 0xff, 0x01, 0x00, 0x00,        // and rax, 0x1ff
                0xbb, 0xe8, 0x03, 0x00, 0x00,        // mov ebx, 1000
                0x48, 0xf7, 0xe3,        // mul rbx
                0x48, 0xc1, 0xe8, 0x09,        // shr rax, 9
                0x48, 0x89, 0xc2,        // mov rdx, rax
                0x66, 0xc7, 0x07, 0x30, 0x30,        // mov word [rdi], "00"
                0x48, 0x83, 0xc7, 0x02,        // add rdi, 2
                0xe8, 0x3e, 0x00, 0x00, 0x00,        // call itoa10
                0xb8, 0x01, 0x00, 0x00, 0x00,        // mov eax, 1
                0x48, 0x89, 0xfa,        // mov rdx, rdi
                0x48, 0x29, 0xe2,        // sub rdx, rsp
                0x4c, 0x01, 0xc2,        // add rdx, r8
                0xbf, 0x01, 0x00, 0x00, 0x00,        // mov edi, 1
                0x48, 0x89, 0xe6,        // mov rsi, rsp
                0x0f, 0x05,        // syscall
                0x48, 0x89, 0xec,        // mov rsp, rbp
                0x5d,        // pop rbp
                0xc3,        // ret
            // </BASE>

        // <CntBytes>
                0x4d, 0x31, 0xc0,        // xor r8, r8
                0x50,        // push rax
            //.loop:
                0x48, 0x31, 0xd2,        // xor rdx, rdx
                0x49, 0xf7, 0xf2,        // div r10
                0x48, 0xff, 0xc7,        // inc rdi
                0x49, 0xff, 0xc0,        // inc r8
                0x48, 0x83, 0xf8, 0x00,        // cmp rax, 0
                0x77, 0xee,        // ja .loop
                0x4c, 0x89, 0xc8,        // mov rax, r9
                0xc6, 0x07, 0x00,        // mov byte [rdi], 0
                0x48, 0xff, 0xcf,        // dec rdi
                0x58,               // pop rax
                0xc3,        // ret
        // </CntBytes>

        // <ITOA10>
                0x4d, 0x31, 0xc0,       // xor r8, r8
            //.Print:
                0x48, 0x31, 0xd2,       // xor rdx, rdx
                0x49, 0xf7, 0xf2,       // div r10
                0x80, 0xc2, 0x30,       // add dl, 0x30
                0x88, 0x17,             // mov byte [rdi], dl
                0x48, 0xff, 0xcf,       // dec rdi
                0x49, 0xff, 0xc0,       // inc r8
                0x48, 0x83, 0xf8, 0x00, // cmp rax, 0
                0x77, 0xe9,             // ja .Print
                0x48, 0xff, 0xc7,       // inc rdi
                0xc3,                   // ret
        // </ITOA10>
    };

static const unsigned char POW_CODE[] =
    {
        // <POW>
            0x48, 0x83, 0xec, 0x10,        // sub rsp, 0x10
            0x48, 0x8b, 0x44, 0x24, 0x08,        // mov rax, qword [rsp + 8]
            0x48, 0x85, 0xc0,        // test rax, rax
            0x74, 0x44,        // je .DontPow
            0x48, 0x83, 0xf8, 0x01,        // cmp rax, 1
            0x74, 0x3e,        // je .DontPow
            0x66, 0x83, 0x3c, 0x24, 0x01,        // cmp word [rsp], 1
            0x74, 0x37,        // je .DontPowButPop
            0xdf, 0x04, 0x24,        // fild word [rsp]
            0x68, 0x00, 0x02, 0x00, 0x00,        // push 0x200
            0xde, 0x34, 0x24,        // fidiv word [rsp]
            0xdf, 0x44, 0x24, 0x10,        // fild word [rsp + 0x10]
            0xde, 0x34, 0x24,        // fidiv word [rsp]
            0xd9, 0xf1,        // fyl2x
            0xdb, 0x54, 0x24, 0xf8,        // fist dword [rsp - 8]
            0xdb, 0x44, 0x24, 0xf8,        // fild dword [rsp - 8]
            0xde, 0xe9,        // fsubp st(1)
            0xd9, 0xf0,        // f2xm1
            0xd9, 0xe8,        // fld1
            0xde, 0xc1,        // faddp st(1)
            0xdb, 0x44, 0x24, 0xf8,        // fild dword [rsp - 8]
            0xd9, 0xc9,        // fxch st(1)
            0xd9, 0xfd,        // fscale
            0xda, 0x0c, 0x24,        // fimul dword [rsp]
            0xdb, 0x5c, 0x24, 0x10,        // fistp dword [rsp + 0x10]

        //.DontPow:
            0x48, 0x83, 0xc4, 0x08,        // add rsp, 8

        //.DontPowButPop:
            0x48, 0x83, 0xc4, 0x08,        // add rsp, 8
            0x58,        // pop rax
            0xc3,        // ret
        // </POW>
    };

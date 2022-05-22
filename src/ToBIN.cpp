#include "ToBIN.h"
#include <sys/mman.h>
#include <unistd.h>
#include "NASM_BIN_TABLE.h"

static int IN_USED  = 0;
static int OUT_USED = 0;

static void PrintA (const char *msg, ...)
{
    fprintf (AsmFile, "%08x:\t\t", ArrLen);

    va_list arg;
    va_start (arg, msg);
    vfprintf (AsmFile, msg, arg);
    va_end (arg);

    fprintf (AsmFile, "\n");
}

static int PrintB (INSTRUCTION ins)
{
    LOG_MSG ("Printing to binary : (0x%08lx) of len (%d) at [0x%08lx]",
             ins.opcode, ins.len, ArrLen);

    memcpy (BinArr + ArrLen, (const char *)(&ins.opcode), ins.len);
    ArrLen += ins.len;

    if (ins.arg_len > 0)
    {
        LOG_MSG ("Printing arg : (0x%08lx) of len (%d)",
                 ins.arg, ins.arg_len);

        memcpy
                (
                    BinArr + ArrLen, (const char *) &(ins.arg),
                    ins.arg_len
                );

        ArrLen += ins.arg_len;
    }

    return 0;
}

static int Bflush (FILE *flushTO)
{
    return fwrite (BinArr, 1, ArrLen, flushTO);
}

static int AddVar (char isConst, int len, TNode *var)
{
    int id = AddId (ASM_IDS, var->data, isConst, len);

    // Listing
    PrintA
    (
        "sub rsp, %d ; declared %.*s; [%d; %d]", // save value to stack
        len * INT_LEN, var->len, var->declared, OFFS (id, 0), OFFS (id, len)
    );

    // save space in stack
    PrintB (SUB_RSP_4_BYTE (len * INT_LEN));

    Curr_rsp += len;

    LOG_MSG ("var declared = %.*s; len = %d; id = %d; "
             "memOfs = %d; Frame = %d; offs = [%d; %d]",
             var->len, var->declared, len, id, IDS[id].memOfs + 1,
             Frame, OFFS (id, 0), OFFS (id, len));

    return id;
}

// ================ <Service Nodes> ===========

static int PrintCallArgs (TNode *node)
{
    if (!RIGHT || !CURR) return 0;

    // Listing
    PrintA ("; call args");

    int pushed = 0;

    for (int pushed = 0; CURR; pushed++)
    {
        NodeToAsm (RIGHT); // Evaluate the argument

        LOG_MSG ("Call arg to stack: |%.*s| to [rsp - %d]",
                 LEN, DECL, 24 + pushed * 8);

        // Listing
        PrintA ("mov [rsp - %d], rax", 24 + pushed * 8);

        // push to stack
        PrintB (MOV_TO_STACK_4_BYTE (-(24 + pushed * 8)));

        CURR = LEFT;
    }

    return pushed;
}

static int PrintCALL (TNode *node)
{
    $ PrintCallArgs (RIGHT);

    int64_t name_hash = LEFT->data;

    // Listing
    PrintA ("call f%ld ; call %.*s",
            abs (name_hash), LEFT->len, LEFT->declared);

    LOG_MSG ("Looking for hash = (%ld)", name_hash);

    // Find addr of called func
    for (int func_iter = 3; func_iter < FuncNum; func_iter++)
    {
        LOG_MSG ("Comparing: name_hash = (%ld) and "
                 "FuncArr[(%d/%d)].hash = (%ld)",
                 name_hash, func_iter, FuncNum - 1, FuncArr[func_iter].hash);

        if (name_hash == FuncArr[func_iter].hash)
        {
            int64_t dest_addr = FuncArr[func_iter].addr;

            // Check include/NASM_BIN_TABLE.h (11) for further explanations
            return PrintB (CALL_NEAR_4_BYTE (dest_addr - ArrLen - 5));
        }
    }

    LOG_ERR ("Hash (%ld): Nothing found!", name_hash);

    return UNDECLARED;
}

static int PrintRET (TNode *node)
{
    $ int rErr = NodeToAsm (RIGHT);
    if (rErr) return rErr;

    // Listing
    MOV_SS ("rsp", "rbp");

    PrintB (MOV_RSP_RBP);

    // Listing
    POP ("rbp ; stack frame return\n");

    PrintB (POP_RBP);

    // Listing
    PrintA ("ret");

    PrintB (RET_1_BYTE);

    return 0;
}

static int PrintDEF (TNode *node)
{
    static int func_iter = 3;

    FuncArr[func_iter].addr = ArrLen;

    func_iter++;

    $ assert (CURR);

    TNode *params = LEFT;

    IdsArr = (Id *) calloc (INIT_IDS_NUM, sizeof (Id));
    IdsNum = 0;

    Frame = 0;

    long hash = abs(params->left->data);
    LOG_MSG ("functon declared: %.*s\n",
             params->left->len, params->left->declared);

    // Listing
    PrintA ("f%ld: ; def %.*s", hash,
              params->left->len, params->left->declared);

    // Listing
    PrintA ("push rbp ; create stack frame");

    PrintB (PUSH_RBP);

    // Listing
    MOV_SS ("rbp", "rsp\n");

    PrintB (MOV_RBP_RSP);

    int params_num = 0;

    for (TNode *curr_param = params->right;
         curr_param;
         curr_param = curr_param->left, params_num++)
    {
        int id = AddId (ASM_IDS, curr_param->right->data, 0, 1);

        LOG_MSG ("param added: %.*s\n"
                 "len = 1; id = %d; memOfs = %d; Frame = %d; offs = [%d; %d]",
                 curr_param->right->len, curr_param->right->declared,
                 id, IDS[id].memOfs + 1, Frame, OFFS (id, 0), OFFS (id, 1));
    }

    // Listing
    PrintA ("sub rsp, %d ; jump over parameters\n", params_num * INT_LEN);
    Curr_rsp += params_num;

    PrintB (SUB_RSP_4_BYTE (params_num * INT_LEN));

    PrintSt (RIGHT);

    Curr_rsp -= params_num;

    free (IdsArr);
    IdsArr = NULL;
    IdsNum = 0;

    return 0;
}

static int PrintIN (TNode *node)
{
    $ if (!node) return 1;

    IN_USED = 1;

    // Listing
    PrintA ("xor rdi, rdi");

    PrintB (XOR_RDI_RDI);

    // Listing
    MOV_SS ("rsi", "inputbuf ; buffer for inputted value\n");

    PrintB (LEA_RSI_4_BYTE (0xC0DEDEAD));

    // Listing
    MOV_SD ("rdx", 15);

    PrintB (MOV_RDX_4_BYTE (15));

    // Listing
    PrintA ("xor rax, rax");

    PrintB (XOR_RAX_RAX);

    // Listing
    PrintA ("syscall");

    PrintB (SYSCALL);

    // Listing
    MOV_SS ("rcx", "rax");

    PrintB (MOV_RCX_RAX);

    // Listing
    PrintA ("call atoi");

    PrintB (CALL_NEAR_4_BYTE (FuncArr[0].addr - ArrLen - 5));

    // Listing
    FLOAT_L ("rax");

    PrintB (SHL_RAX_1_BYTE (NUMS_AFTER_POINT));

    return 0;
}

static int PrintOUT (TNode *node)
{
    $ if (!node) return 1;

    OUT_USED = 1;

    NodeToAsm (RIGHT);

    // Listing
    PrintA ("call out");

    PrintB (CALL_NEAR_4_BYTE (0xD0DE));

    return 0;
}

static int PrintWHILE (TNode *node)
{
    static int whileNum = 0;
    int    init_rsp     = Curr_rsp;
    int    init_var_num = IdsNum;

    // Listing
    MOV_SS ("r12", "rsp ; save rsp to rcx");

    PrintB (MOV_R12_RSP);

    int localWhileNum = whileNum;
    whileNum++;

	//Listing
    PrintA ("; while");

    int whileStartAddr = ArrLen;

	//Listing
    PrintA (".%dwhile:", localWhileNum);
    NodeToAsm (LEFT);

    // Listing
    PrintA ("test rax, rax");

    PrintB (TEST_RAX_RAX);

	//Listing
    PrintA ("je .%dwhileEnd", localWhileNum);

    PrintB (JE_1_BYTE (0));
    int jeArgPos = ArrLen - 1;

    if (RIGHT)
        NodeToAsm (RIGHT);

    //Listing
    MOV_SS ("rsp", "r12 ; forget any variables created during the loop")

    PrintB (MOV_RSP_R12);

	//Listing
    PrintA ("jmp .%dwhile", localWhileNum);

    PrintB (JMP_4_BYTE (whileStartAddr - ArrLen - 5));

	//Listing
    PrintA (".%dwhileEnd:", localWhileNum);

    BinArr[jeArgPos] = (unsigned char) (ArrLen - jeArgPos - 1);

    RmId (ASM_IDS, IdsNum - init_var_num);

    Curr_rsp = init_rsp;

    return 0;
}

static int PrintIF (TNode *node)
{
    $ static int ifNum = 0;
    int localIfNum = ifNum;
    ifNum++;

    // Listing
    PrintA ("; if statement");

    NodeToAsm (LEFT);

    // Listing
    PrintA ("test rax, rax");

    PrintB (TEST_RAX_RAX);

    // Listing
    PrintA ("je .%dfalse\n", localIfNum);

    PrintB (JE_1_BYTE (0));

    int jeArgPos = ArrLen - 1;

    TNode *decis = RIGHT;

    if (decis->left)
        NodeToAsm (decis->left);

    // Listing
    PrintA ("jmp .%denif\n", localIfNum);

    PrintB (JMP_4_BYTE (0));

    int jmpArgPos = ArrLen - 4;

    // Listing
    PrintA (".%dfalse:\n", localIfNum);

    BinArr[jeArgPos] = (unsigned char) (ArrLen - jeArgPos - 1);

    if (decis->right)
        NodeToAsm (decis->right);

    // Listing
    PrintA (".%denif:\n", localIfNum);

    BinArr[jmpArgPos] = (int32_t) (ArrLen - jmpArgPos - 1);

    return 0;
}

#define IF_SERVICE(serv)                                                        \
    if (DATA == ServiceNodes[serv]) return Print##serv (CURR);
static int PrintSERV (TNode *node)
{
    $ IF_SERVICE (IF);
    $ IF_SERVICE (DEF);
    $ IF_SERVICE (RET);
    $ IF_SERVICE (OUT);
    $ IF_SERVICE (IN);
    $ IF_SERVICE (CALL);
    $ IF_SERVICE (WHILE);

    return 0;
}
#undef IF_SERVICE

// ================= <Operators> =============

#define OP_CASE(op, act)                                                        \
    case op:                                                                    \
        res = NodeToAsm (RIGHT);                                                \
        /* Listing */                                                           \
        PUSH ("rax\n");                                                         \
        PrintB (PUSH_RAX);                                                      \
                                                                                \
        res += NodeToAsm (LEFT);                                                \
        /* Listing */                                                           \
        POP ("rbx\n");                                                          \
        PrintB (POP_RBX);                                                       \
                                                                                \
        /* Listing */                                                           \
        PrintA (#act " rax, rbx\n");                                            \
        PrintB (act##_RAX_RBX);                                                 \
                                                                                \
        break;

#define COMP_CASE(val, action)                                                  \
    case val: Comp (#action, action##_1_BYTE (8), node); break;

static int PrintOP (TNode *node)
{
    int res = 0;
    switch (node->data)
    {
        case '=':
            res = PrintAssn (CURR);
            break;
        case '!':
            res = PrintNeg();
            break;
        COMP_CASE (EE,  JE);
        COMP_CASE (AE,  JGE);
        COMP_CASE (BE,  JLE);
        COMP_CASE (NE,  JNE);
        COMP_CASE ('>', JG);
        COMP_CASE ('<', JL);
        OP_CASE   ('+', ADD);
        OP_CASE   ('-', SUB);

        case '*':
        {
            res = PrintMul (CURR);
            break;
        }

        case '/':
        {
            res = PrintDiv (CURR);
            break;
        }

        case '^':
        {
            PrintPow (CURR);
            break;
        }

        default:
            LOG_ERR ("Unknown operand: %c\n", (char) node->data);
            break;
    }

    return res;
}
#undef OP_CASE
#undef COMP_CASE

static int Comp (const char *action, INSTRUCTION jmp_ins, TNode *node)
{
    static int cmpNum = 0;

    // Listing
    PrintA ("; %s", action);

    NodeToAsm (LEFT);
    // Listing
    MOV_SS ("rbx", "rax ; save left to rbx");

    PrintB (MOV_RBX_RAX);

    NodeToAsm (RIGHT);

    // Listing
    PrintA ("cmp rbx, rax");

    PrintB (CMP_RBX_RAX);

    // Listing
    PrintA ("%s .%dcmp\n", action, cmpNum);

    // (JE/JGE/JLE/JG/JL/JNE)_1_BYTE (8) <-- 8 = len of xor + jmp
    PrintB (jmp_ins);

    // <False>
    PrintA ("xor rax, rax ; false");

    PrintB (XOR_RAX_RAX);

    // Listing
    PrintA ("jmp .%dcmpEnd\n", cmpNum);

    PrintB (JMP_4_BYTE (5));
    // </False>

    // <True>
    PrintA (".%dcmp:", cmpNum);

    // Listing
    PrintA ("mov rax, 1 ; true\n");

    PrintB (MOV_RAX_4_BYTE (1));
    // </True>

    // Listing
    PrintA (".%dcmpEnd:\n", cmpNum);

    cmpNum++;

    return 0;
}

static int PrintNeg (void)
{
    // Listing
    PrintA ("not rax");

    PrintB (NOT_RAX);

    return 0;
}

static int PrintPow (TNode *node)
{
///////////////////////////////////////////////
    int err = NodeToAsm (LEFT);
    if (err) return err;

    PrintA ("test rax, rax");
    PrintA ("jz .DontPow");

    PrintA ("cmp rax, 1");
    PrintA ("je .DontPow");

    PUSH ("rax\n");

    err = NodeToAsm (RIGHT);
    if (err) return err;

    PrintA ("cmp rax, 1");
    PrintA ("je .DontPowButPop");

    PUSH ("rax\n");

    // Load args into FPU stack
    PrintA ("fild  WORD [rsp]            ; load base onto FPU stack");
    PrintA ("fidiv DWORD [const_for_pow] ; convert from pseudo-float\n");

    PrintA ("fild  WORD [rsp + %d]      ; load power onto FPU stack", INT_LEN);
    PrintA ("fidiv DWORD [const_for_pow] ; convert from pseudo-float\n");

    PrintA ("fyl2x ; power * log_2_(base)\n");

    PrintA ("; value between -1 and 1 is required by pow of 2 command");
    PrintA ("fist DWORD [rsp - 8] ; cast to int");
    PrintA ("fild DWORD [rsp - 8] ;");
    PrintA ("fsub      ; fit into [-1; 1]\n");

    PrintA ("f2xm1 ; 2^(power * log_2_(base)) - 1 = base^power\n");

    PrintA ("fld1   ; push 1");
    PrintA ("fadd   ; add 1 to the result\n");

    PrintA ("fild DWORD [rsp - 8] ; load casted value");
    PrintA ("fxch   ; exchange st(0) <-> st(1)");
    PrintA ("fscale ; multiply by remaining power of 2");

    PrintA ("fimul DWORD [const_for_pow] ; to pseudo-float");
    PrintA ("fistp DWORD [rsp + %d]      ; save pow value to stack\n", INT_LEN);

    ADD_SD ("rsp", INT_LEN);

    PrintA (".DontPowButPop:");

    POP ("rax");

    PrintA (".DontPow:");

    return 0;
}

static int PrintDiv (TNode *node)
{
    int err = NodeToAsm (RIGHT);
    if (err) return err;

    // Listing
    PUSH ("rax\n");

    PrintB (PUSH_RAX);

    err = NodeToAsm (LEFT);
    if (err) return err;

    // Listing
    POP ("rbx\n");

    PrintB (POP_RBX);

    // Listing
    FLOAT_R ("rbx");

    PrintB (SAR_RBX_1_BYTE (NUMS_AFTER_POINT));

    // Listing
    PrintA ("cqo\n");

    PrintB (CQO_1_BYTE);

    // Listing
    PrintA ("idiv rbx\n");

    PrintB (IDIV_RBX);

    return 0;
}

static int PrintMul (TNode *node)
{
    int err = NodeToAsm (RIGHT);
    if (err) return err;

    // Listing
    PUSH ("rax\n");

    PrintB (PUSH_RAX);

    err = NodeToAsm (LEFT);
    if (err) return err;

    // Listing
    POP ("rbx\n");

    PrintB (POP_RBX);

    // Listing
    PrintA ("imul rbx\n");

    PrintB (IMUL_RBX);

    // Listing
    FLOAT_R ("rax");

    PrintB (SAR_RAX_1_BYTE (NUMS_AFTER_POINT));

    return 0;
}

static int PrintAssn (TNode *node)
{
    LOG_MSG ("%.*s = %.*s", LEFT->len, LEFT->declared,
                            RIGHT->len, RIGHT->declared);

    int id_pos = FindId (ASM_IDS, LEFT->data);

    int len = 0;     // if we declare an array
    if (LEFT->right)
        len = (int) LEFT->right->data;

    int rErr = NodeToAsm (RIGHT);
    if (rErr) return rErr;

    if (id_pos < Frame)
    {
        char isConst = 0;
        if (LEFT->left) isConst = 1;

        if (len < 1)
        {
            len = 1;
        }

        id_pos = AddVar (isConst, len, LEFT);

        len--;
    }

    // Listing
    PrintA ("mov [rbp - %d], rax ; %.*s = rax",
            OFFS (id_pos, len), LEFT->len, LEFT->declared);

    PrintB (MOV_RBP_4_BYTE_RAX (-OFFS (id_pos, len)));

    return 0;
}

// ================ <Const, ID, Var> ==========

static int PrintConst (TNode *node)
{
    // Listing
    PrintA ("mov rax, %d ; const value << 9", DATA << NUMS_AFTER_POINT);

    PrintB (MOV_RAX_4_BYTE (DATA << NUMS_AFTER_POINT));

    return 0;
}

static int PrintID (TNode *node)
{
    PrintA ("; %.*s", LEN, DECL);

    if (LEFT)
    {
        $ int lErr = NodeToAsm (LEFT);
        if (lErr) return lErr;
    }

    if (RIGHT)
    {
        $ int rErr = NodeToAsm (RIGHT);
        if (rErr) return rErr;
    }

    return 0;
}

static int PrintVar (TNode *node)
{
    LOG_MSG ("Looking for hash = %ld; name = %.*s\n", DATA, LEN, DECL);
    int id_pos = FindId (ASM_IDS, DATA);
    LOG_MSG ("Hash found on pos = %d\n", id_pos);

    if (id_pos >= Frame)
    {
        int len = 0;

        if (RIGHT)
        {
            len = (int) RIGHT->data;
        }

        // Listing
        PrintA ("mov rax, [rbp - %d] ; %.*s",
                OFFS (id_pos, len), LEN, DECL);

        PrintB (MOV_RAX_RBP_4_BYTE (-OFFS (id_pos, len)));
    }
    else
    {
        SyntaxErr ("Undeclared variable used: %.*s\n", LEN, DECL);
        return UNDECLARED;
    }

    return 0;
}

static int PrintSt (TNode *node)
{
    if (LEFT)
    {
        LOG_MSG ("LEFT_NODE_Enter: <%p>", LEFT);

        $ int lErr = NodeToAsm (LEFT);

        LOG_MSG ("LEFT_NODE_Exit: <%p>", LEFT);

        if (lErr) return lErr;
    }

    LOG_MSG ("RIGHT_NODE_Enter: <%p>", RIGHT);

    $ int rErr = NodeToAsm (RIGHT);

    LOG_MSG ("RIGHT_NODE_Exit: <%p>", RIGHT);

    PrintA ("");

    return rErr;
}

// ================ <Other> ===================

static int NodeToAsm (TNode *node)
{
    $ assert (CURR);

    switch (TYPE)
    {
        case TYPE_STATEMENT:
            $ return PrintSt (CURR);
        case TYPE_VAR:
            $ return PrintVar (CURR);
        case TYPE_ID:
            $ return PrintID (CURR);
        case TYPE_CONST:
            $ return PrintConst (CURR);
        case TYPE_OP:
            $ return PrintOP (CURR);
        case TYPE_SERVICE:
            $ return PrintSERV (CURR);
        default:
            break;
    }

    return OK;
}

// The following 2 functions are total cringe, however their implementation
// is acceptable
static void PrintSTD_OUT (void)
{
    PrintA (
        "section .data\n\n"

        "outArr: db 10, \">> \"\n"
        "outBig: dq 0, 0\n"
        "outDot: db \'.\'\n"
        "outLow: dq 0, 0 \n\n"

        "section .text\n\n"
        ";==============================================\n"
        "; StdLib: out\n"
        "; Expects:\n"
        ";   outbuffer\n"
        ";   rax - value\n"
        "; Returns: None\n"
        ";==============================================\n\n"

        "out:\n"
        "    mov rdi, outBig\n"
        "    cmp rax, 0\n"
        "    jge .NotNegative\n"
        "    mov BYTE [outBig], '-'\n"
        "    inc rdi\n"
        "    neg rax\n"

        ".NotNegative:\n"
        "    push rax\n"
        "    shr rax, %d\n"
        "    mov rdx, rax\n"
        "    call itoa10\n"

        "    mov rax, 0x01\n"
        "    mov rdi, 0x01\n"
        "    mov rsi, outArr\n"
        "    mov rdx, r8\n"
        "    add rdx, 5\n"
        "    syscall\n"

        "    pop rax\n"
        "    and rax, %d\n"
        "    mov rbx, 1000\n"
        "    mul rbx\n"
        "    shr rax, %d\n"
        "    mov rdx, rax\n"
        "    mov rdi, outLow\n"
        "    call itoa10\n"

        "    mov rax, 0x01\n"
        "    mov rdi, 0x01\n"
        "    mov rsi, outDot\n"
        "    mov rdx, r8\n"
        "    add rdx, 1\n"
        "    syscall\n"

        "    ret\n"

        ";==============================================\n"
        "; StdLib: itoa\n"
        ";==============================================\n\n"

        "CountBytes:\n"
        "	xor rax, rax\n"
        "        mov rax, rdx	; save value in r10 to count symbols in it\n"
        "        xor ch, ch\n"
        ".Loop:\n"
        "        inc ch  	; bytes counter\n"
        "        shr rax, cl     ; rax >> cl\n"
        "        jnz .Loop\n"
        "	xor rax, rax\n"
        "        mov al, ch\n"
        "ret\n"

        ";==============================================\n"
        "; Converts integer value into a string, base 10\n"
        "; Expects:\n"
        ";       rdx - Integer value\n"
        ";       rdi - Buffer to write into\n"
        "; Returns:\n"
        ";       r8  - Printed bytes num\n"
        "; Destr:\n"
        ";       rdx, r10, r9\n"
        ";==============================================\n"

        "itoa10:\n"
            "xor r8, r8		; r8 = bytes counter\n"
            "mov r9, rdx 		; from now on, value is stored in r9\n"
                "mov rax, rdx		; save value to rax\n"
                "mov r10, 10\n"
        ".CntBytes:              	; skips, bytes that are required to save the value\n"
                "xor rdx, rdx		; reset remaining\n"
                "div r10            ; rax = rax / 10; rdx = rax % 10\n"
                "inc rdi\n"
                "inc r8\n"
                "cmp rax, 0000h\n"
                "ja .CntBytes\n"
                "mov rax, r9           	; reset value\n"
                "mov byte [rdi], 00\n"
                "dec rdi\n"
        ".Print:\n"
                "xor rdx, rdx\n"
                "div r10                ; rax = rax / 10; rdx = rax % 10\n"
                "add dl, '0'           	; to ASCII\n"
                "mov [rdi], dl\n"
                "dec rdi\n"
                "cmp rax, 00h\n"
                "ja .Print\n"
                "; rdi = &buffer - 1\n"
                "inc rdi ; rdi = &buffer\n"
                "ret\n\n",
        NUMS_AFTER_POINT, (1 << NUMS_AFTER_POINT) - 1, NUMS_AFTER_POINT);

    return;
}

static void PrintSTD_IN (void)
{
    PrintA(
        "section .bss\n\n"

        "inputbuf: resq 2\n\n"

        "section .text\n\n"

        ";==============================================\n"
        "; StdLib: atoi\n"
        "; Expects:\n"
        ";     rsi - inputbuf\n"
        ";     rcx - len of input\n"
        "; Returns:\n"
        ";     rax - result int\n"
        ";==============================================\n"

        "atoi:\n"
        "    xor rax, rax\n"
        "    xor rbx, rbx\n"
        "    dec rcx\n"
        "    jz .End\n"
        "    cmp BYTE [rsi], \'-\'\n"
        "    jne .Loop\n"
        "    inc rsi\n"
        "    dec rcx\n"
        ".Loop:\n"
        "    mov bl, [rsi]\n"
        "    sub rbx, \'0\'\n"
        "    inc rsi\n"
        "    mov rdx, 10\n"
        "    mul rdx\n"
        "    add rax, rbx\n"
        "    loop .Loop\n"
        ".End:"
        "    cmp BYTE [inputbuf], \'-\'\n"
        "    jne .Ret\n"
        "    neg rax\n"
        ".Ret:\n"
        "    ret\n\n");

    return;
}

static int ReadyBuf (void)
{
    int pagesize = (int) sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1)
    {
        LOG_ERR ("Pagesize unknown: %d\n", pagesize);
        return 0;
    }

    BinArr = (char *) aligned_alloc (pagesize, pagesize * sizeof (char));
    if (!BinArr)
    {
        LOG_ERR ("Unable to allocate memory of len = %d\n", INIT_CAP);
        return MEM_ALLOC_ERR;
    }

    memset (BinArr, 0x00, pagesize * sizeof (char));

    ArrCap = pagesize;
    ArrLen = 0;

    return 0;
}

int ToBIN (TNode *root, const char *name, int func_num, FuncId *func_ids)
{
    assert (root);
    assert (name);

    FuncArr = func_ids;
    FuncNum = func_num;

    DBINT;

    int bufReady = ReadyBuf();

    AsmFile = fopen ("Listing.lst", "wt");
    if (!AsmFile)
    {
        LOG_ERR ("Unable to open asm file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

// ---------------- <Translation> -------------

    DBINT;

    PrintA ("const_for_pow: dq 512");
    PrintB (INSTRUCTION { 0x512, 8, 0, 0});

    PRERENDER (PUSH_EVERYTING);

    PrintA ("call main");
    PrintB (CALL_NEAR_4_BYTE (0));

    int main_arg_offs = ArrLen - 4;

    PRERENDER (POP_EVERYTING);

    PrintA ("ret");
    PrintB (RET_1_BYTE);

    FuncArr[0].addr = ArrLen;
    PRERENDER (IN_CODE);

    FuncArr[1].addr = ArrLen;
    // PRERENDER (OUT_CODE);

    FuncArr[2].addr = ArrLen;
    // PRERENDER (POW_CODE);

    int err = NodeToAsm (root);

    if (FuncArr[FuncNum - 1].hash == MAIN_HASH)
    {
        int64_t dest_addr = FuncArr[FuncNum - 1].addr;

        LOG_MSG ("MAIN_HASH (%ld) Found in FuncArr[%d] at offs (%d)",
                 MAIN_HASH, FuncNum - 1, dest_addr);

        LOG_MSG ("Inputting main jmp offset: writing (%d) to BinArr[0x%08lx]",
                 dest_addr - main_arg_offs - 4, main_arg_offs);

        // Check include/NASM_BIN_TABLE.h (11) for further explanations
        *(int32_t*) (BinArr + main_arg_offs) = dest_addr - main_arg_offs - 4;
    }
    else
    {
        LOG_ERR ("MAIN_HASH (%ld) not found!!!", MAIN_HASH);
        free (BinArr);
        fclose (AsmFile);
        return UNDECLARED;
    }

// ---------------- <Testing> -----------------

    err += mprotect (BinArr, ArrCap, PROT_EXEC | PROT_READ);
    if (err) printf ("Node to asm: errors occured: %d", err);

    DBINT;

    int (*testFunc) (void) = (int (*) (void)) (BinArr + 8);
    testFunc();

    LOG_MSG ("test func result = %d", 4);

    mprotect (BinArr, ArrCap, PROT_READ | PROT_WRITE);

// ---------------- <Finishing> ---------------

    DBINT;

    FILE *BinFile = fopen (name, "wt");
    if (!BinFile)
    {
        printf ("Unable to open asm file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

    Bflush (BinFile);

    if (AsmFile)
        fclose (AsmFile);

    if (BinFile)
        fclose (BinFile);

    free (BinArr);

    return err;
}

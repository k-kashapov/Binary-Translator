#include "ToBIN.h"

static int IN_USED  = 0;
static int OUT_USED = 0;

static void PrintA (const char *msg, ...)
{
    for (int tab = 0; tab < Tabs; tab++)
    {
        fputc ('\t', AsmFile);
    }

    va_list arg;
    va_start (arg, msg);
    vfprintf (AsmFile, msg, arg);
    va_end (arg);
    fprintf (AsmFile, "\n");
}

static int PrintB (INSTRUCTION *ins)
{
    if (ArrCap < ArrLen + ins->len + 1)
    {
        void *tmp = realloc (BinArr, ArrCap * 2);
        if (!tmp) return MEM_ALLOC_ERR;

        BinArr = (char *) tmp;
        ArrCap *= 2;
    }

    printf ("printing to bin\n");

    sprintf (BinArr + ArrLen, "%.*s", ins->len, (const char *)(&ins->opcode));
    ArrLen += ins->len;

    if (ins->arg_len > 0)
    {
        printf ("arg to bin\n");
        sprintf (BinArr + ArrLen, "%.*s", ins->arg_len, (const char *) &(ins->arg));
        ArrLen += ins->arg_len;
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

    PrintA
    (
        "sub rsp, %d ; declared %.*s; [%d; %d]", // save value to stack
        len * INT_LEN, var->len, var->declared, OFFS (id, 0), OFFS (id, len)
    );

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

    PrintA ("; call args");

    int pushed = 0;

    for (int pushed = 0; CURR; pushed++)
    {
        NodeToAsm (RIGHT); // Evaluate the argument

        // push to stack
        PrintA ("mov [rsp - %d], rax", 24 + pushed * 8);

        CURR = LEFT;
    }

    return pushed;
}

static int PrintCALL (TNode *node)
{
    $ PrintCallArgs (RIGHT);

    PrintA ("call f%ld ; call %.*s",
            abs (LEFT->data), LEFT->len, LEFT->declared);

    return 0;
}

static int PrintRET (TNode *node)
{
    $ int rErr = NodeToAsm (RIGHT);
    if (rErr) return rErr;

    MOV_SS ("rsp", "rbp");

    POP ("rbp ; stack frame return\n");

    PrintA ("ret");

    return 0;
}

static int PrintDEF (TNode *node)
{
    $ assert (CURR);

    TNode *params = LEFT;

    IdsArr = (Id *) calloc (INIT_IDS_NUM, sizeof (Id));
    IdsNum = 0;

    Frame = 0;

    long hash = abs(params->left->data);
    LOG_MSG ("functon declared: %.*s\n",
             params->left->len, params->left->declared);

    $ PrintA ("f%ld: ; def %.*s", hash,
              params->left->len, params->left->declared);

    Tabs++;

    PrintA ("push rbp ; create stack frame");
    MOV_SS ("rbp", "rsp\n");

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

    PrintA ("sub rsp, %d ; jump over parameters\n", params_num * INT_LEN);
    Curr_rsp += params_num;

    PrintSt (RIGHT);

    Curr_rsp -= params_num;

    Tabs--;

    free (IdsArr);
    IdsArr = NULL;
    IdsNum = 0;

    return 0;
}

static int PrintIN (TNode *node)
{
    $ if (!node) return 1;

    IN_USED = 1;

    PrintA ("xor rdi, rdi");
    MOV_SS ("rsi", "inputbuf ; buffer for inputted value\n");
    MOV_SD ("rdx", 15);

    PrintA ("xor rax, rax");
    PrintA ("syscall");

    MOV_SS ("rcx", "rax");
    PrintA ("call atoi");
    FLOAT_L ("rax");

    return 0;
}

static int PrintOUT (TNode *node)
{
    $ if (!node) return 1;

    OUT_USED = 1;

    NodeToAsm (RIGHT);

    PrintA ("call out");

    return 0;
}

static int PrintWHILE (TNode *node)
{
    static int whileNum = 0;
    int    init_rsp     = Curr_rsp;
    int    init_var_num = IdsNum;

    MOV_SS ("r12", "rsp ; save rsp to rcx");

    int localWhileNum = whileNum;
    whileNum++;

    PrintA ("; while");
    Tabs++;

    PrintA (".%dwhile:", localWhileNum);
    NodeToAsm (LEFT);

    PrintA ("test rax, rax");
    PrintA ("je .%dwhileEnd", localWhileNum);

    if (RIGHT)
        NodeToAsm (RIGHT);

    MOV_SS ("rsp", "r12 ; forget any variables created during the loop");
    PrintA ("jmp .%dwhile", localWhileNum);

    PrintA (".%dwhileEnd:", localWhileNum);
    Tabs--;

    RmId (ASM_IDS, IdsNum - init_var_num);

    Curr_rsp = init_rsp;

    return 0;
}

static int PrintIF (TNode *node)
{
    $ static int ifNum = 0;
    int localIfNum = ifNum;
    ifNum++;

    PrintA ("; if statement");
    Tabs++;
    NodeToAsm (LEFT);

    PrintA ("test rax, rax");
    PrintA ("je .%dfalse\n", localIfNum);

    TNode *decis = RIGHT;

    if (decis->left)
        NodeToAsm (decis->left);
    PrintA ("jmp .%denif\n", localIfNum);

    PrintA (".%dfalse:\n", localIfNum);
    if (decis->right)
        NodeToAsm (decis->right);

    PrintA (".%denif:\n", localIfNum);
    Tabs--;

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
        Tabs++;                                                                 \
\
        res = NodeToAsm (RIGHT);                                                \
        PUSH ("rax\n");                                                         \
\
        res += NodeToAsm (LEFT);                                                \
        POP ("rbx\n");                                                          \
\
        PrintA (act " rax, rbx\n");                                             \
\
        Tabs--;                                                                 \
        break;

#define COMP_CASE(val, action) case val: Comp (action, node); break;

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
        COMP_CASE (EE, "je");
        COMP_CASE (AE, "jge");
        COMP_CASE (BE, "jle");
        COMP_CASE (NE, "jn");
        COMP_CASE ('>', "jg");
        COMP_CASE ('<', "jl");
        OP_CASE   ('+', "add");
        OP_CASE   ('-', "sub");

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

static int Comp (const char *action, TNode *node)
{
    static int cmpNum = 0;

    PrintA ("; %s", action);
    Tabs++;

    NodeToAsm (LEFT);
    MOV_SS ("rbx", "rax ; save left to rbx");

    NodeToAsm (RIGHT);
    PrintA ("cmp rbx, rax");

    PrintA ("%s .%dcmp\n", action, cmpNum);

    // false
    PrintA ("xor rax, rax ; false");
    PrintA ("jmp .%dcmpEnd\n", cmpNum);

    // true
    PrintA (".%dcmp:", cmpNum);
    PrintA ("mov rax, 1 ; true\n");

    PrintA (".%dcmpEnd:\n", cmpNum);

    Tabs--;
    cmpNum++;

    return 0;
}

static int PrintNeg (void)
{
    PrintA ("not rax");

    return 0;
}

static int PrintPow (TNode *node)
{
    Tabs++;

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

    Tabs--;

    return 0;
}

static int PrintDiv (TNode *node)
{
    Tabs++;

    int err = NodeToAsm (RIGHT);
    if (err) return err;

    PUSH ("rax\n");

    err = NodeToAsm (LEFT);
    if (err) return err;

    POP ("rbx\n");
    FLOAT_R ("rbx");

    PrintA ("cqo\n");

    PrintA ("idiv rbx\n");

    Tabs--;
    return 0;
}

static int PrintMul (TNode *node)
{
    Tabs++;

    int err = NodeToAsm (RIGHT);
    if (err) return err;

    PUSH ("rax\n");

    err = NodeToAsm (LEFT);
    if (err) return err;

    POP ("rbx\n");

    PrintA ("imul rbx\n");

    FLOAT_R ("rax");

    Tabs--;

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

    PrintA ("mov [rbp - %d], rax ; %.*s = rax",
            OFFS (id_pos, len), LEFT->len, LEFT->declared);

    return 0;
}

// ================ <Const, ID, Var> ==========

static int PrintConst (TNode *node)
{
    PrintA ("mov rax, %d ; const value << 9", DATA << NUMS_AFTER_POINT);

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

        PrintA ("mov rax, [%s - %d] ; %.*s",
                RBP, OFFS (id_pos, len), LEN, DECL);
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

static int PrintUNARY (TNode *node)
{
    if (!RIGHT) return 1;
    NodeToAsm (RIGHT);

    size_t func_num = sizeof (UnaryFuncs) / sizeof (*UnaryFuncs);

    for (size_t func = 0; func < func_num; func++)
    {
        if (DATA == UnaryFuncs[func])
        {
            PrintA ("%.*s", LEN, DECL);
            return 0;
        }
    }

    return 1;
}

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
        case TYPE_UNARY:
            $ return PrintUNARY (CURR);
        default:
            break;
    }

    PrintA ("; node (%.*s) of type %d", LEN, DECL, node->type);

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

int ToBIN (TNode *root, const char *name)
{
    // main func hash = f1058

    BinArr = (char *) calloc (sizeof (char), INIT_CAP);
    if (!BinArr)
    {
        LOG_ERR ("Unable to allocate memory of len = %d\n", INIT_CAP);
        return MEM_ALLOC_ERR;
    }

    ArrCap = INIT_CAP;
    ArrLen = 0;

    //
    // PrintA ("global _start\n"
    //
    //         "section .data\n\n"
    //
    //         "const_for_pow: dd 0x200        ; memory for float computations\n"
    //
    //         "section .text\n\n"
    //
    //         "_start:\n"
    //         "\tpush rbx   ; push everything\n"
    //         "\tpush rbp   ; push everything\n"
    //         "\tpush r12   ; push everything\n"
    //         "\tpush r13   ; push everything\n"
    //         "\tpush r14   ; push everything\n"
    //         "\tpush r15   ; push everything\n\n"
    //
    //         "\tcall f1058 ; call main\n\n"
    //
    //         "\tpop rbx   ; restore initial regs state\n"
    //         "\tpop rbp   ; restore initial regs state\n"
    //         "\tpop r12   ; restore initial regs state\n"
    //         "\tpop r13   ; restore initial regs state\n"
    //         "\tpop r14   ; restore initial regs state\n"
    //         "\tpop r15   ; restore initial regs state\n\n"
    //
    //         "\tmov rdi, rax\n"
    //         "\tmov rax, 0x3C\n"
    //         "\tsyscall\n");
    //
    // IN_USED = 0;
    // OUT_USED = 0;
    //
    // int err = NodeToAsm (root);
    //
    // if (OUT_USED)
    // {
    //     PrintSTD_OUT();
    // }
    //
    // if (IN_USED)
    // {
    //     PrintSTD_IN();
    // }

    AsmFile = fopen (name, "wt");
    if (!AsmFile)
    {
        LOG_ERR ("Unable to open asm file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

    INSTRUCTION ins = { 0xC3C3C3C3, 4, 0, 0 };

    int err = PrintB (&ins);
    Bflush (AsmFile);

    fclose (AsmFile);

    if (err) printf ("Node to asm: errors occured: %d", err);

    return err;
}

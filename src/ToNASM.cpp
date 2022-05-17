#include "ToNASM.h"

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

static int PopVar (int len)
{
    int id = RmId (ASM_IDS, 1);

    ADD_SD ("rsp", INT_LEN * len);

    Curr_rsp -= len;

    LOG_MSG ("Popped variable of len %d; new memOfs = %d",
             len, IDS[id].memOfs + 1);

    PrintA ("add rsp, %d; removed variable of len %d", len * INT_LEN, len);

    return 0;
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
    $ int pushed = PrintCallArgs (RIGHT);

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
    NodeToAsm (RIGHT);

    PrintA ("lea rdi, [out_str]\n");

    MOV_SS  ("rsi", "rax");
    FLOAT_R ("rsi");

    PrintA ("and rax, %d ; mask for last 9 bits", (1 << NUMS_AFTER_POINT) - 1);
    MOV_SS ("rcx", "3E8h ; rcx = 1000d for translation to base 10");
    PrintA ("mul rcx");
    PrintA ("shr rax, %d", NUMS_AFTER_POINT);
    MOV_SS ("rdx", "rax\n");

    PrintA ("xor rax, rax");
    PrintA ("call printf\n");

    MOV_SS ("rdi", "[stdout]");
    PrintA ("call fflush\n");

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

// ================= <Operations> =============

#define OP_CASE(op, act)                                                        \
    case op:                                                                    \
        Tabs++;                                                                 \
        NodeToAsm (RIGHT);                                                      \
        PUSH ("rax\n");                                                         \
        NodeToAsm (LEFT);                                                       \
        POP ("rbx\n");                                                          \
        PrintA (act " rax, rbx\n");                                             \
        Tabs--;                                                                 \
        break;

#define COMP_CASE(val, action) case val: Comp (action, node); break;

static int PrintOP (TNode *node)
{
    switch (node->data)
    {
        case '=':
            return PrintAssn (CURR);
            break;
        case '!':
            return PrintNeg (CURR);
            break;
        COMP_CASE (EE, "je");
        COMP_CASE (AE, "jae");
        COMP_CASE (BE, "jbe");
        COMP_CASE (NE, "jn");
        COMP_CASE ('>', "ja");
        COMP_CASE ('<', "jb");
        OP_CASE   ('+', "add");
        OP_CASE   ('-', "sub");

        case '*':
        {
            Tabs++;

            NodeToAsm (RIGHT);
            PUSH ("rax\n");

            NodeToAsm (LEFT);
            POP ("rbx\n");

            PrintA ("imul rbx\n");

            FLOAT_R ("rax");

            Tabs--;
            break;
        }

        case '/':
        {
            Tabs++;

            NodeToAsm (RIGHT);
            PUSH ("rax\n");

            NodeToAsm (LEFT);
            POP ("rbx\n");
            FLOAT_R ("rbx");

            MOV_SD ("rdx", 0);

            PrintA ("div rbx\n");

            Tabs--;
            break;
        }

        case '^':
        {
            Tabs++;

            NodeToAsm (LEFT);
            PrintA ("test rax, rax");
            PrintA ("jz .DontPow");

            PrintA ("cmp rax, 1");
            PrintA ("je .DontPow");

            PUSH ("rax\n");

            NodeToAsm (RIGHT);

            PrintA ("cmp rax, 1");
            PrintA ("je .DontPowButPop");

            PUSH ("rax\n");

            // Load args into FPU stack
            PrintA ("fild  WORD [rsp + %d]      ; load power onto FPU stack", INT_LEN);
            PrintA ("fidiv DWORD [const_for_pow] ; convert from pseudo-float\n");

            PrintA ("fild  WORD [rsp]           ; load base onto FPU stack");
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
            break;
        }

        default:
            LOG_ERR ("Unknown operand: %c\n", (char) node->data);
            break;
    }

    return 0;
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

static int PrintNeg (TNode *node)
{
    PrintA ("not rax");

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

int ToNASM (TNode *root, const char *name)
{
    AsmFile = fopen (name, "wt");
    if (!AsmFile)
    {
        LOG_ERR ("Unable to open asm file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

    // main func hash = f1058

    PrintA ("global _start\n"
            "extern printf, scanf, pow, fflush, stdout\n"

            "section .bss\n\n"

            "inputbuf: resq 2\n\n"

            "section .data\n\n"

            "const_for_pow: dd 0x200        ; memory for float computations\n"
            "in_str:  db \"%%d\"             ; format string for scanf\n"
            "out_str: db \">> %%d.%%d\", 0xA  ; format string for printf\n\n"

            "section .text\n\n"

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
                    "ret\n\n"

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
            "    ret\n\n"

            "_start:\n"
            "\tpush rbx   ; push everything\n"
            "\tpush rbp   ; push everything\n"
            "\tpush r12   ; push everything\n"
            "\tpush r13   ; push everything\n"
            "\tpush r14   ; push everything\n"
            "\tpush r15   ; push everything\n\n"

            "\tcall f1058 ; call main\n\n"

            "\tpop rbx   ; restore initial regs state\n"
            "\tpop rbp   ; restore initial regs state\n"
            "\tpop r12   ; restore initial regs state\n"
            "\tpop r13   ; restore initial regs state\n"
            "\tpop r14   ; restore initial regs state\n"
            "\tpop r15   ; restore initial regs state\n\n"

            "\tmov rdi, rax\n"
            "\tmov rax, 0x3C\n"
            "\tsyscall\n");

    int err = NodeToAsm (root);
    if (err) printf ("Node to asm: errors occured: %d", err);

    return err;
}

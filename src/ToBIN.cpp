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

    #ifdef LOGGING
    PrintB (INT_3);
    #endif

    int64_t name_hash = LEFT->data;

    // Listing
    PrintA ("call f%ld ; call %.*s",
            abs (name_hash), LEFT->len, LEFT->declared);

    LOG_MSG ("Looking for hash = (%ld), |%.*s|", name_hash, LEFT->len, LEFT->declared);

    // Find addr of called func
    for (int func_iter = 3; func_iter < FuncNum; func_iter++)
    {
        LOG_MSG ("Comparing: name_hash = (%ld) and "
                 "FuncArr[(%d/%d)].hash = (%ld), addr = (0x%08lx)",
                 name_hash, func_iter, FuncNum - 1, FuncArr[func_iter].hash, FuncArr[func_iter].addr);

        if (name_hash == FuncArr[func_iter].hash)
        {
            int64_t dest_addr = FuncArr[func_iter].addr;

            LOG_MSG ("Found hash on [%d]. Printing call from (0x%08lx) "
                     "to (0x%08lx), arg = (0x%08lx)",
                     func_iter, ArrLen + 5, dest_addr, dest_addr - ArrLen - 5);

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

    LOG_MSG ("Function added |%.*s| at FuncArr[%d].addr = (0x%08lx) / .hash = (%d)",
             LEFT->left->len, LEFT->left->declared, func_iter, ArrLen, LEFT->left->data);

    FuncArr[func_iter++].addr = ArrLen;

    $ assert (CURR);

    TNode *params = LEFT;

    IdsArr = (Id *) calloc (INIT_IDS_NUM, sizeof (Id));
    IdsNum = 0;

    Frame = 0;

    long hash = abs(params->left->data);

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

    PrintA ("call in");

    LOG_MSG ("Printing IN call from (0x%08lx) to (0x%08lx), arg = (0x%08lx)",
             ArrLen, FuncArr[0].addr, FuncArr[0].addr - ArrLen - 5);

    PrintB (CALL_NEAR_4_BYTE (FuncArr[0].addr - ArrLen - 5));

    return 0;
}

static int PrintOUT (TNode *node)
{
    $ if (!node) return 1;

    OUT_USED = 1;

    NodeToAsm (RIGHT);

    // Listing
    PrintA ("call out");

    LOG_MSG ("Printing OUT call from (0x%08lx) to (0x%08lx), arg = (0x%08lx)",
             ArrLen, FuncArr[1].addr, FuncArr[1].addr - ArrLen - 5);

    PrintB (CALL_NEAR_4_BYTE (FuncArr[1].addr - ArrLen - 5));

    return 0;
}

static int PrintWHILE (TNode *node)
{
    static int whileNum = 0;
    int    init_rsp     = Curr_rsp;
    int    init_var_num = IdsNum;

    // Listing
    MOV_SS ("r12", "rsp ; save rsp to r12");

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

    PrintB (JE_4_BYTE (0));
    int jeArgPos = ArrLen - 4;

    if (RIGHT)
        NodeToAsm (RIGHT);

    //Listing
    MOV_SS ("rsp", "r12 ; forget any variables created during the loop")

    PrintB (MOV_RSP_R12);

	//Listing
    PrintA ("jmp .%dwhile", localWhileNum);

    const int ARG_LEN = 4;
    const int OP_LEN  = 4;

    PrintB (JMP_4_BYTE (whileStartAddr - (ArrLen + OP_LEN + ARG_LEN)));

	//Listing
    PrintA (".%dwhileEnd:", localWhileNum);

    *(int32_t *) (BinArr + jeArgPos) = (int32_t) (ArrLen - jeArgPos - ARG_LEN);

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

    PrintB (JE_4_BYTE (0));

    int jeArgPos = ArrLen - 4;

    TNode *decis = RIGHT;

    if (decis->left)
        NodeToAsm (decis->left);

    // Listing
    PrintA ("jmp .%denif\n", localIfNum);

    PrintB (JMP_4_BYTE (0));

    int jmpArgPos = ArrLen - 4;

    // Listing
    PrintA (".%dfalse:\n", localIfNum);

    LOG_MSG ("Printing je { from (0x%08lx) to (0x%08lx) }: arg (0x%08lx) to [0x%08lx]",
             jeArgPos + 4, ArrLen, (int32_t) (ArrLen - (jeArgPos + 4)), jeArgPos);

    *(int32_t *)(BinArr + jeArgPos) = (int32_t) (ArrLen - (jeArgPos + 4));

    if (decis->right)
        NodeToAsm (decis->right);

    // Listing
    PrintA (".%denif:\n", localIfNum);

    LOG_MSG ("Printing jmp arg (0x%08lx) to [0x%08lx]",
             (int32_t) (ArrLen - (jmpArgPos + 4)), jmpArgPos);

    *(int32_t *)(BinArr + jmpArgPos) = (int32_t) (ArrLen - (jmpArgPos + 4));

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
    case val: Comp (#action, action##_4_BYTE (8), node); break;

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

    // (JE/JGE/JLE/JG/JL/JNE)_4_BYTE (8) <-- 8 = len of xor + jmp
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
    int err = NodeToAsm (LEFT);
    if (err) return err;

    PrintB (MOV_TO_STACK_4_BYTE (-16));

    err = NodeToAsm (RIGHT);
    if (err) return err;

    PrintB (MOV_TO_STACK_4_BYTE (-24));

    PrintA ("call pow");

    PrintB (CALL_NEAR_4_BYTE (FuncArr[2].addr - ArrLen - 5));

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

    int len = 0;     // in case we declare an array
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

static int ReadyBuf (void)
{
    int pagesize = (int) sysconf (_SC_PAGE_SIZE);
    
    if (pagesize == -1)
    {
        LOG_ERR ("Pagesize unknown: %d\n", pagesize);
        return MEM_ALLOC_ERR;
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

    int bufErr = ReadyBuf();
    if (bufErr) return bufErr;

    AsmFile = fopen ("Listing.lst", "wt");
    if (!AsmFile)
    {
        LOG_ERR ("Unable to open asm file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

// ---------------- <Translation> -------------

    DBINT;

    PRERENDER (PUSH_EVERYTING);

    PrintA ("call main");
    PrintB (CALL_NEAR_4_BYTE (0));

    int main_arg_offs = ArrLen - 4;

    PRERENDER (POP_EVERYTING);

    PRERENDER (EXIT);

    FuncArr[0].addr = ArrLen;
    PRERENDER (IN_CODE);

    FuncArr[1].addr = ArrLen;
    PRERENDER (OUT_CODE);

    FuncArr[2].addr = ArrLen;
    PRERENDER (POW_CODE);

    LOG_MSG ("written prerenders to file, bytes: %ld\n", ArrLen);

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

/*
    err += mprotect (BinArr, ArrCap, PROT_EXEC | PROT_READ);
    if (err) printf ("Node to asm: errors occured: %d", err);

    DBINT;

    int (*testFunc) (void) = (int (*) (void)) (BinArr + 8);
    testFunc();

    LOG_MSG ("test func result = %d", 4);

    mprotect (BinArr, ArrCap, PROT_READ | PROT_WRITE);
*/

// ---------------- <Finishing> ---------------

    DBINT;

    const char *bin_name = "anek.exe";

    FILE *BinFile = CreateTemplate (bin_name, ArrLen);
    if (!BinFile)
    {
        printf ("Unable to open bin file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

    Bflush   (BinFile);
    CloseEXE (BinFile, bin_name);

    if (AsmFile)
        fclose (AsmFile);

    free (BinArr);

    return err;
}

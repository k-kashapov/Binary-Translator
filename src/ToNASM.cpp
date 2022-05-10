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

    PrintA
    (
        "mov [%s - %d], rax ; declared %.*s", // save value to FREE + OFFSET
        FREE, (IdsNum + 1) * 8, var->len, var->declared
    );

    int id = AddId (ASM_IDS, var->data, isConst, len);

    LOG_MSG ("var declared = %.*s; len = %d; id = %d", var->len, var->declared, var->len, id);

    return id;
}

// ================ <Service Nodes> ===========

static int PrintCallArgs (TNode *node)
{
    if (!RIGHT || !CURR) return 0;

    PrintA ("; call args");

    for (int curr_register = 0; curr_register < MaxArgsRegs && CURR; curr_register++)
    {
        NodeToAsm (RIGHT); // Evaluate the argument

        // mov arg to register for fastcall
        MOV_SS (GeneralRegs[VarRegsNum - curr_register], "rax");

        CURR = LEFT;
    }

    int pushed = 0;

    while (CURR)
    {
        pushed++;
        NodeToAsm (RIGHT);
        PUSH ("rax");         // pass using the stack
    }

    return pushed;
}

static int PrintCALL (TNode *node)
{
    $ int pushed = PrintCallArgs (RIGHT);

    PrintA ("call f%ld ; call %.*s",
            abs (LEFT->data), LEFT->len, LEFT->declared);

    if (pushed)
        ADD_SD ("rsp", pushed * 8);

    return 0;
}

static int PrintRET (TNode *node)
{
    $ int rErr = NodeToAsm (RIGHT);
    if (rErr) return rErr;

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

    long hash = abs(params->left->data);
    LOG_MSG ("functon declared: %.*s\n", params->left->len, params->left->declared);

    $ PrintA ("f%ld: ; def %.*s", hash, params->left->len, params->left->declared);

    Tabs++;

    PrintA ("push rbp ; create stack frame");
    MOV_SS ("rbp", "rsp\n");

    for (TNode *curr_param = params->right;
         curr_param;
         curr_param = curr_param->left)
    {
        AddId (ASM_IDS, curr_param->right->data);
        LOG_MSG ("param added: %.*s\n", curr_param->right->len, curr_param->right->declared);
    }

    PrintSt (RIGHT);

    Tabs--;

    free (IdsArr);
    IdsArr = NULL;
    IdsNum = 0;

    return 0;
}

static int PrintIN (TNode *node)
{
    $ if (!node) return 1;
    PrintA ("; in ??????????????????????????");
    return 0;
}

static int PrintOUT (TNode *node)
{
    $ if (!node) return 1;
    NodeToAsm (RIGHT);

    PrintA ("; out !!!!!!!!!!!!!!!!!!!!!!!!");

    return 0;
}

static int PrintWHILE (TNode *node)
{
    static int whileNum = 0;
    int localWhileNum = whileNum;
    whileNum++;

    PrintA ("; while");
    Tabs++;

    PrintA ("%dwhile:", localWhileNum);
    NodeToAsm (LEFT);

    PrintA ("test rax, rax");
    PrintA ("jz %dwhileEnd", localWhileNum);

    if (RIGHT)
        NodeToAsm (RIGHT);

    PrintA ("jmp %dwhile", localWhileNum);

    PrintA ("%dwhileEnd:", localWhileNum);
    Tabs--;

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
    PrintA ("jz %dfalse", localIfNum);

    TNode *decis = RIGHT;

    if (decis->left)
        NodeToAsm (decis->left);
    PrintA ("jmp %denif", localIfNum);

    PrintA ("%dfalse:", localIfNum);
    if (decis->right)
        NodeToAsm (decis->right);

    PrintA ("%denif:", localIfNum);
    Tabs--;

    return 0;
}

#define IF_SERVICE(serv) if (DATA == ServiceNodes[serv]) return Print##serv (CURR);
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
        NodeToAsm (LEFT);                                                       \
        NodeToAsm (RIGHT);                                                      \
        PrintA (act);                                                           \
        break

#define COMP_CASE(val, action) case val: Comp (action, node, cmpNum); break;

static int PrintOP (TNode *node)
{
    $ static int cmpNum = 0;
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
        OP_CASE ('+', "add");
        OP_CASE ('-', "sub");
        OP_CASE ('*', "mul");
        OP_CASE ('/', "div");
        OP_CASE ('^', "pow");
        default:
            LOG_ERR ("Unknown operand: %c\n", (char) node->data);
            break;
    }

    return 0;
}
#undef OP_CASE
#undef COMP_CASE

static int Comp (const char *action, TNode *node, int cmpNum)
{
    PrintA ("; %s", action);
    Tabs++;

    NodeToAsm (LEFT);
    TNode left_var = { 0, TYPE_CONST, "temp left", 9, NULL, NULL, NULL };
    int left_id = AddVar (0, 1, &left_var);

    NodeToAsm (RIGHT);

    PrintA ("cmp [rsp - %d * 8], rax", left_id);

    PrintA ("%s %dcmp\n", action, cmpNum);

    // false
    PrintA ("xor rax, rax ; false");
    PrintA ("jmp %dcmpEnd\n", cmpNum);

    // true
    PrintA ("%dcmp:", cmpNum);
    PrintA ("mov rax, 1 ; true");

    PrintA ("%dcmpEnd:\n", cmpNum);
    RmId (ASM_IDS, 1);

    Tabs--;
    cmpNum++;

    return 0;
}

static int PrintNeg (TNode *node)
{
    PrintA ("not rax ; DOES THIS WORK? I THINK NOT");

    return 0;
}

static int PrintAssn (TNode *node)
{
    $ int rErr = NodeToAsm (RIGHT);

    LOG_MSG ("%.*s = %s", LEFT->len, LEFT->declared, RIGHT->declared);

    if (rErr) return rErr;

    int id_pos = FindId (ASM_IDS, LEFT->data);

    int len = 0;     // if we declare an array
    if (LEFT->right)
        len = (int) LEFT->right->data;

    if (id_pos >= 0)
    {
        int offset = id_pos + len;

        PrintA ("mov [%s - %d], rax ; %.*s = rax", FREE, offset * 8, LEFT->len, LEFT->declared);
    }
    else
    {
        char isConst = 0;
        if (LEFT->left) isConst = 1;

        if (len < 1)
        {
            len = 1;
        }

        AddVar (isConst, len, LEFT);
    }

    return 0;
}

// ================ <Const, ID, Var> ==========

static int PrintConst (TNode *node)
{
    PrintA ("mov rax, %d ; const value", DATA);

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

    if (id_pos >= 0)
    {
        int offset = id_pos;

        if (RIGHT && RIGHT->data != 0)
        {
            offset += (int) RIGHT->data;
        }

        PrintA ("mov rax, [%s - %d] ; %.*s", FREE, offset * 8, LEN, DECL);
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
    for (size_t func = 0; func < sizeof (UnaryFuncs) / sizeof (*UnaryFuncs); func++)
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
            "section .text\n"

            "_start:\n"
            "\tpusha      ; push everything\n"
            "\tcall f1058 ; call main\n"
            "\tpopa       ; restore initial regs state\n"

            "\tmov rax, 0x3C\n"
            "\txor rdi, rdi\n"
            "\tsyscall\n");

    int err = NodeToAsm (root);
    if (err) printf ("Node to asm: errors occured: %d", err);

    return err;
}

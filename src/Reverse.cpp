#include "Reverse.h"

static void PrintA (const char *msg, ...)
{
    $$ va_list arg;
    va_start (arg, msg);
    vfprintf (AnekFile, msg, arg);
    va_end (arg);
    fprintf (AnekFile, " ");
}

static int PrintCallArgs (TNode *node)
{
    $ if (!RIGHT || !CURR) return 0;

    do {
        NodeToAnek (RIGHT);
        CURR = LEFT;
    } while (CURR);

    PrintA (".\n");

    return 0;
}

static int PrintCALL (TNode *node)
{
    $ PrintA ("Анекдот: Заходят как - то в бар");

    PrintCallArgs (RIGHT);

    PrintA ("А бармен им говорит: %.*s.", LEFT->len, LEFT->declared);

    return 0;
}

static int PrintRET (TNode *node)
{
    $ PrintA ("Козырная");

    $ if (RIGHT)
    {
        NodeToAnek (RIGHT);
    }

    PrintA (", господа.");

    return 0;
}

static int PrintDEF (TNode *node)
{
    $ assert (CURR);

    TNode *params = LEFT;

    IdsArr = (Id *) calloc (INIT_IDS_NUM, sizeof (Id));
    IdsNum = 0;

    LOG_MSG ("functon declared: %.*s\n", params->left->len, params->left->declared);
    PrintA ("Господа , а не сыграть ли нам в новую игру.\n");

    PrintVar (params->left);

    PrintA ("называется.\n");

    if (params->right)
    {
        PrintA ("Правила очень просты:");

        for (TNode *curr_param = params->right;
             curr_param;
             curr_param = curr_param->left)
        {
            PrintA ("%.*s", curr_param->right->len, curr_param->right->declared);
            AddId (ANEK_IDS, curr_param->right->data);
            LOG_MSG ("param added: %.*s\n", curr_param->right->len, curr_param->right->declared);
        }

        PrintA (".\n");
    }

    Tabs++;

    PrintA ("Алга\n");
    PrintSt (RIGHT);
    PrintA ("Развернулся и алга.\n");

    Tabs--;

    free (IdsArr);
    IdsNum = 0;

    return 0;
}

static int PrintIN (TNode *node)
{
    $ if (!node) return 1;
    PrintA ("ввод пользователем числового значения с клавиатуры");
    return 0;
}

static int PrintOUT (TNode *node)
{
    $ if (!node) return 1;
    PrintA ("Голос,");
    NodeToAnek (RIGHT);
    PrintA (".");

    return 0;
}

static int PrintNeg (TNode *node)
{
    $ PrintA ("Нифига не");
    NodeToAnek (RIGHT);

    return 0;
}


static int PrintIF (TNode *node)
{
    $ PrintA ("Кто прочитал");

    NodeToAnek (LEFT);
    PrintA ("тот сдохнет.\n");

    Tabs++;

    TNode *decis = RIGHT;

    if (decis->left)
        NodeToAnek (decis->left);

    PrintA ("Ставь лайк\n");

    if (decis->right)
        NodeToAnek (decis->right);

    Tabs--;

    PrintA ("и можешь считать, что не читал.");

    return 0;
}

static int PrintWHILE (TNode *node)
{
    $ PrintA ("В дверь постучали");

    NodeToAnek (LEFT);
    PrintA ("раз.\n");

    Tabs++;

    if (RIGHT)
        NodeToAnek (RIGHT);
    Tabs--;

    PrintA ("Дверь отвалилась.");

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

static int PrintOP (TNode *node)
{
    $ switch (node->data)
    {
        case '=':
            return PrintAssn (CURR);
        case '!':
            return PrintNeg (CURR);
        case '+': [[fallthrough]];
        case '-': [[fallthrough]];
        case '/': [[fallthrough]];
        case '<': [[fallthrough]];
        case '>': [[fallthrough]];
        case '^': [[fallthrough]];
        case EE:  [[fallthrough]];
        case AE:  [[fallthrough]];
        case BE:  [[fallthrough]];
        case NE:
            PrintA ("Биба");
            NodeToAnek (LEFT);
            PrintA ("%.*s", LEN, DECL);
            break;
        case '*':
            PrintA ("Биба");
            NodeToAnek (LEFT);
            PrintA ("дофига");
            break;
        default:
            break;
    }

    NodeToAnek (RIGHT);
    PrintA ("Боба");
    return 0;
}
#undef OP_CASE
#undef COMP_CASE

static int PrintDECL (TNode *node)
{
    $ PrintA ("\"");

    PrintA ("%.*s", LEFT->len, LEFT->declared);

    char isConst = 0;

    if (LEFT->left)
    {
        PrintA ("всегда");
        NodeToAnek (RIGHT);
        isConst = 1;
    }

    int len = 1;
    if (LEFT->right)
        len = (int) LEFT->right->data;

    if (len < 1)
    {
        SyntaxErr ("Attempting to allocate array of size %d < 1, %s\n", len, LEFT->declared);
        return ZERO_CAP_DECL;
    }

    PrintA ("\" - подумал Штирлиц %d раз.", len);

    LOG_MSG ("var declared = %.*s; len = %d\n", LEFT->len, LEFT->declared, LEFT->len);
    if (!IdsArr)
    {
        AddId (CONST_IDS, LEFT->data, 1, len);
    }
    else
    {
        AddId (ANEK_IDS, LEFT->data, isConst, len);
    }

    return isConst;
}

static int PrintAssn (TNode *node)
{
    $ int decl = FindId (ANEK_IDS, LEFT->data);
    if (decl < 0)
    {
        LOG_MSG ("Undeclared variable used: %.*s\nDeclaring...\n", LEN, DECL);
        $ int isConst = PrintDECL (CURR);
        if (isConst) return 0;
    }

    $ PrintA ("Этим");

    int lErr = NodeToAnek (LEFT);

    PrintA ("был");
    int rErr = NodeToAnek (RIGHT);
    PrintA (".");

    return rErr + lErr;
}

static int PrintConst (TNode *node)
{
    PrintA ("%d", DATA);

    return 0;
}

static int PrintID (TNode *node)
{
    PrintA ("; этого быть не должно %.*s\n", LEN, DECL);

    if (LEFT)
    {
        $ int lErr = NodeToAnek (LEFT);
        if (lErr) return lErr;
    }

    if (RIGHT)
    {
        $ int rErr = NodeToAnek (RIGHT);
        if (rErr) return rErr;
    }

    return 0;
}

static int PrintVar (TNode *node)
{
    LOG_MSG ("Looking for const = %ld; name = %.*s\n", DATA, LEN, DECL);
    int id_pos = FindId (CONST_IDS, DATA);

    if (id_pos < 0)
    {
        LOG_MSG ("Looking for hash = %ld; name = %.*s\n", DATA, LEN, DECL);
        id_pos = FindId (ANEK_IDS, DATA);
    }

    LOG_MSG ("Hash found on pos = %d\n", id_pos);

    if (!RIGHT || RIGHT->data == 0)
    {
        PrintA ("%.*s", LEN, DECL);
    }
    else
    {
        PrintA ("%.*s (автору %d лет)", LEN, DECL, RIGHT->data);
    }

    if (id_pos < 0)
    {
        return UNDECLARED;
    }

    return 0;
}

static int PrintSt (TNode *node)
{
    if (LEFT)
    {
        $ int lErr = NodeToAnek (LEFT);
        if (lErr) return lErr;
    }

    $ int rErr = NodeToAnek (RIGHT);

    PrintA ("\n\n");

    return rErr;
}

static int NodeToAnek (TNode *node)
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

    PrintA ("; node (%.*s) of type %d", LEN, DECL, node->type);

    return OK;
}

int Reverse (TNode *root, const char *name)
{
    AnekFile = fopen (name, "wt");
    if (!AnekFile)
    {
        LOG_ERR ("Unable to open anek file; name = %s\n", name);
        return OPEN_FILE_FAILED;
    }

    ConstArr = (Id *) calloc (INIT_IDS_NUM, sizeof (Id));
    ConstNum = 0;

    PrintA ("Купил мужик шляпу.\n");

    int err = NodeToAnek (root);
    if (err) printf ("Node to anek: errors occured: %d\n", err);

    PrintA ("А она ему как раз, господа.");

    return err;
}

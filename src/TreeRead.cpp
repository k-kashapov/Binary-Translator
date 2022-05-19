#include "Logs.h"
#include "Lang.h"

enum READ_ERRS
{
    CONST_READ_FAIL = 0x201,
    UNKNOWN_SYMBOL  = 0x202,
    LR_EXIST        = 0x203,
};

struct Reader
{
    char *src;
    TNode *curr_node;
    TNode *init_node;
};

#define CURR rdr->curr_node

int ProcessChar (Reader *rdr);

static int ProcessAlpha (Reader *rdr)
{
    int type = TYPE_ID;

    if (*rdr->src == '\'')
    {
        type = TYPE_VAR;
        rdr->src++;
    }

    const char *declared  = rdr->src;
    int64_t    hash       = 0;
    int        bytes_read = 0;

    while (IsAlpha (*rdr->src))
    {
        bytes_read++;
        hash += *rdr->src * bytes_read;
        rdr->src++;
    }

    if (hash == ServiceNodes[STMT])
    {
        type = TYPE_STATEMENT;
    }
    else
    {
        for (int srvc = 0; srvc < SRVC_NUM; srvc++)
        {
            if (hash == ServiceNodes[srvc])
            {
                type = TYPE_SERVICE;
                break;
            }
        }
    }

    CURR->type     = type;
    CURR->len      = (int) (rdr->src - declared);
    CURR->data     = hash;
    CURR->declared = declared;

    if (type == TYPE_VAR && *rdr->src == '\'') rdr->src++; // skip \" after var name
    printf ("end variant: %.*s\n", CURR->len, CURR->declared);

    return ProcessChar (rdr);
}

static int ProcessNum (Reader *rdr)
{
    double val    = 0;
    int bytes_read = 0;

    int read = sscanf (rdr->src, "%lf%n", &val, &bytes_read);
    if (!read)
    {
        LOG_ERR ("Const value read err: src = %s", rdr->src);
        return CONST_READ_FAIL;
    }

    printf ("Const value read = %lf\n", val);

    CURR->data     = '/';
    CURR->type     = TYPE_OP;
    CURR->declared = "/";
    CURR->len      = 1;
    CURR->left     = CreateNode ((int64_t) (val * 1000), TYPE_CONST);
    CURR->right    = CreateNode (1000, TYPE_CONST);

    rdr->src += bytes_read;

    return ProcessChar (rdr);
}

static int ProcessSpecial (Reader *rdr)
{
    switch (*rdr->src)
    {
        case '(':
            if (!CURR->left)
            {
                CURR->left         = CreateNode (0);
                CURR->left->parent = CURR;
                CURR               = CURR->left;
            }
            else if (!CURR->right)
            {
                CURR->right         = CreateNode (0);
                CURR->right->parent = CURR;
                CURR                = CURR->right;
            }
            else
            {
                LOG_ERR ("Node %p L and R already exist: %s\n", CURR, rdr->src);
                return LR_EXIST;
            }
            break;
        case ')':
            CURR = CURR->parent;
            if (rdr->src[1] == ')' && !CURR->right) // checking if parent has only one child
            {
                CURR->right = CURR->left;
                CURR->left  = NULL;
            }
            break;
        case '!': [[fallthrough]];
        case '=': [[fallthrough]];
        case '>': [[fallthrough]];
        case '<':
            printf ("Comparison char = %c\n", rdr->src[0]);
            if (rdr->src[1] == '=')
            {
                printf ("next one is \'=\'\n");
                CURR->data = *rdr->src + 2 * '=';
                CURR->type = TYPE_OP;
                CURR->declared = rdr->src;
                CURR->len = 2;
                rdr->src++;
                break;
            }
            [[fallthrough]];
        case '^': [[fallthrough]];
        case '+': [[fallthrough]];
        case '-': [[fallthrough]];
        case '*': [[fallthrough]];
        case '/':
            CURR->data     = (tree_elem) (*rdr->src);
            CURR->type     = TYPE_OP;
            CURR->declared = rdr->src;
            CURR->len      = 1;
            break;
        default:
            LOG_ERR ("Unknown symbol: %c (%d)\n", *rdr->src, *rdr->src);
            return UNKNOWN_SYMBOL;
    }

    rdr->src++;

    return ProcessChar (rdr);
}

int ProcessChar (Reader *rdr)
{
    printf ("---------\n"
            "curr char is %c (%d)\n", *rdr->src, *rdr->src);

    while (isspace (*rdr->src)) rdr->src++;

    if (IsAlpha (*rdr->src) || *rdr->src == '\'')
    {
        $ return ProcessAlpha (rdr);
    }
    else if (*rdr->src >= '0' && *rdr->src <= '9')
    {
        $ return ProcessNum (rdr);
    }
    else if (*rdr->src == '\0')
    {
        $ return 0;
    }
    else
    {
        $ return ProcessSpecial (rdr);
    }
}

TNode *BuildTreeFromBase (Config *io_config, char **buffer)
{
    char *source = read_file (io_config->input_file);
    if (!source)
    {
        LOG_ERR ("in function: %s (%d) couldn't read source file\n",
                __FUNCTION__, __LINE__);
        return NULL;
    }

    long src_len = (long) strlen (source);
    printf ("Length = %ld\n", src_len);

    Reader rdr    = {};
    rdr.src       = source + 1;
    rdr.curr_node = CreateNode (0, TYPE_STATEMENT);
    rdr.init_node = rdr.curr_node;

    $ int errors = ProcessChar (&rdr);
    if (errors)
    {
        LOG_MSG ("Errors were detected during file reading. Stopping...\n", 0);
        CreateNodeImage (rdr.init_node, "fail.png");

        free (source);
        DestructNode (rdr.init_node);

        return NULL;
    }

    CreateNodeImage (rdr.init_node, "init_graph.png");

    *buffer = source;

    return rdr.init_node;
}

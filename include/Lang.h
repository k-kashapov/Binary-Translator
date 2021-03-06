#ifndef LANG_H
#define LANG_H

#include "Logs.h"
#include <stdarg.h>
#include "files.h"
#include "Tree.h"
#include "grammarEnum.h"

const int INIT_IDS_NUM = 10;

const char NotAlpha[] = "0123456789:-+!?*/ \"\n\t\r(){}[]^\\,.=<>\'";
const int  AlphaNum   = sizeof (NotAlpha) / sizeof (char);
const int  MAIN_HASH  = 1058;

struct Id
{
    int64_t hash;
    char    isConst;
    int     len;
    int     memOfs;
};

struct FuncId
{
    int64_t hash;
    int64_t addr;
};

struct Trans
{
    Id     *IdsArr;
    int    IdsNum;
    TNode  **tok;
    FuncId *FuncArr;
    int    FuncsNum;
    int    ce;
};

enum LANG_EXIT_CODES
{
    REQUIRE_FAIL  = 0x01,
    REDECLARATION = 0x02,
    UNDECLARED    = 0x03,
    ZERO_CAP_DECL = 0x04,
    MEM_ALLOC_ERR = 0x05,
};

int64_t SimpleHash (const void *data, int len);

#define SERVICE_HASH(val) SimpleHash (val, strlen (val))
const int64_t ServiceNodes[] =
{
    SERVICE_HASH ("define"),
    SERVICE_HASH ("function"),
    SERVICE_HASH ("parameter"),
    SERVICE_HASH ("call"),
    SERVICE_HASH ("if"),
    SERVICE_HASH ("decision"),
    SERVICE_HASH ("while"),
    SERVICE_HASH ("return"),
    SERVICE_HASH ("print"),
    SERVICE_HASH ("scan"),
    SERVICE_HASH ("statement")
};
#undef SERVICE_HASH

const int SRVC_NUM = sizeof (ServiceNodes) / sizeof (ServiceNodes[0]);

enum ServiceHash
{
    DEF = 0,
    FUNC,
    PARAM,
    CALL,
    IF,
    DECISION,
    WHILE,
    RET,
    OUT,
    IN,
    STMT,
};

#define TRANS_IDS &trans->IdsArr, &trans->IdsNum

#define HASH_EQ(node, hash_num) (node->data == ServiceNodes[hash_num])
#define ST(l, r)                CreateNode (0, TYPE_STATEMENT, NULL, l, r)
#define IDEXISTS(target)        (FindId (TRANS_IDS, target) >= 0)


enum COND_EXP
{
    AE  = '>' + 2 * '=',
    BE  = '<' + 2 * '=',
    NE  = '!' + 2 * '=',
    EE  = 3 * '=',
    OR  = 3 * '|',
    AND = 3 * '&',
};


TNode *BuildTreeFromBase (Config *io_config, char **buffer);

int IsAlpha (char val);

TNode *GetSt (Trans *trans, const char *end_cond);

TNode *GetN (Trans *trans);

TNode *GetID (Trans *trans);

TNode *GetP (Trans *trans);

TNode *GetT (Trans *trans);

TNode *GetSum (Trans *trans);

TNode *GetCmp (Trans *trans);

TNode *GetNeg (Trans *trans);

TNode *GetE (Trans *trans);

TNode *GetPow (Trans *trans);

TNode *Assn (Trans *trans);

TNode *GetG (Trans *trans, int *ce);

TNode *GetRet (Trans *trans);

TNode *GetFunc (Trans *trans);

TNode *GetCall (Trans *trans);

TNode *GetIF (Trans *trans);

TNode *GetWhile (Trans *trans);

TNode *GetOP (Trans *trans);

int SyntaxErr (const char *msg, ...);

TNode *GetDec (Trans *trans);

TNode *ReadToken (const char *str);

TNode **LexicAnalysis (const char *string, int *nodesNum);

void OpenGraphFile (const char *name);

void PrintNodeDot (TNode *node);

int CreateNodeImage (TNode *node, const char *name);

void CloseGraphFile (void);

void OpenBaseFile (const char *name);

int SaveNode (TNode *node, const char *name);

void CloseBaseFile (void);

int Translate (TNode *root, const char *name);

int ToNASM (TNode *root, const char *name);

int ToBIN (TNode *root, const char *name, int func_num, FuncId *func_ids);

void FreeTransTree (TNode *root, TNode **nodes, int nodesNum);

int AddId (Id **IdsArr, int *IdsNum, int64_t hash, char isConst = 0, int len = 1, int memOfs = 0);

int FindId (Id **IdsArr, int *IdsNum, int64_t hash, int reqOfs = -1);

int RmId (Id **IdsArr, int *IdsNum, int num = 1);

int Reverse (TNode *root, const char *name = "reverse.anek");

#endif

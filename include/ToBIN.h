#ifndef TOBIN_H
#define TOBIN_H

#define LOG_NAME "ToBIN"
#include "Lang.h"

// Found this on github: RustamSubkhankulov/BinaryTranslator
#ifdef LOGGING
    #define DBINT __asm__ ("int 3");
#else
    #define DBINT ;
#endif

struct INSTRUCTION
{
    int64_t opcode  = 0;
    int     len     = 0;
    int     arg_len = 0;
    int64_t arg     = 0;
};

static FILE *AsmFile   = NULL;
static int  IdsNum     = 0;
static Id   *IdsArr    = NULL;
static int  Curr_rsp   = 0;
static int  Frame      = 0;
static char *BinArr    = NULL;
static int  ArrLen     = 0;
static int  ArrCap     = 0;

// initial binary array capacity = page size
const static int INIT_CAP = 512;
const static int INT_LEN  = 8;   // length of single int

static int  PrintB        (INSTRUCTION ins);
static int  Bflush        (FILE *flushTO);
static int  AddVar        (char isConst, int len, TNode *var);
static int  Comp          (const char *action, TNode *node);
static int  PrintCallArgs (TNode *node);
static int  PrintCALL     (TNode *node);
static int  PrintRET      (TNode *node);
static int  PrintDEF      (TNode *node);
static int  PrintIN       (TNode *node);
static int  PrintOUT      (TNode *node);
static int  PrintNeg      (void);
static int  PrintPow      (TNode *node);
static int  PrintDiv      (TNode *node);
static int  PrintMul      (TNode *node);
static int  PrintWHILE    (TNode *node);
static int  PrintSERV     (TNode *node);
static int  PrintOP       (TNode *node);
static int  PrintAssn     (TNode *node);
static int  PrintConst    (TNode *node);
static int  PrintID       (TNode *node);
static int  PrintVar      (TNode *node);
static int  PrintSt       (TNode *node);
static void PrintSTD_OUT  (void);
static void PrintSTD_IN   (void);
static int  ReadyBuf      (void);
static int  NodeToAsm     (TNode *node);

#define CURR    node
#define LEFT    node->left
#define RIGHT   node->right
#define DATA    node->data
#define DECL    node->declared
#define LEN     node->len
#define TYPE    node->type
#define IDS     IdsArr
#define IDNUM   IdsNum

#define OFFS(id_, len_) (IDS[id_].memOfs + len_ - Frame) * INT_LEN

#define ASM_IDS    &IDS,       &IDNUM
#define GLOBAL_IDS &GlobalArr, &GlobalNum

#define RAX "rax" // ret value
#define RBP "rbp"
#define RSP "rsp"

#define ADD_SS(dst, src) PrintA ("add %s, %s", dst, src); // args: str, str
#define ADD_SD(dst, src) PrintA ("add %s, %d", dst, src); // args: str, num
#define SUB_SD(dst, src) PrintA ("sub %s, %d", dst, src); // args: str, num
#define MOV_SS(dst, src) PrintA ("mov %s, %s", dst, src);
#define MOV_SD(dst, src) PrintA ("mov %s, %d", dst, src);

#define SAR(tgt, num) PrintA ("sar %s, %d ; pseudo-float emul", tgt, num);
#define SHL(tgt, num) PrintA ("shl %s, %d ; pseudo-float emul", tgt, num);

#define NUMS_AFTER_POINT 9

#define FLOAT_R(tgt) SAR (tgt, NUMS_AFTER_POINT)
#define FLOAT_L(tgt) SHL (tgt, NUMS_AFTER_POINT)

#define INC(trgt) PrintA ("inc %s", trgt);
#define PUSH(src) PrintA ("push %s", src);
#define POP(dst)  PrintA ("pop %s", dst);

#endif

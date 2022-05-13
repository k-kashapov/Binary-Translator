#ifndef TONASM_H
#define TONASM_H

#define LOG_NAME "ToNASM"
#include "Lang.h"

static FILE *AsmFile   = NULL;
static int  IdsNum     = 0;
static Id   *IdsArr    = NULL;
static int  Tabs       = 0;
static int  Curr_rsp   = 0;
static int  Frame      = 0;

const static int INT_LEN = 8;

static void PrintA        (const char *msg, ...);
static int  AddVar        (char isConst, int len, TNode *var);
static int  PopVar        (int len);
static int  Comp          (const char *action, TNode *node);
static int  PrintCallArgs (TNode *node);
static int  PrintCALL     (TNode *node);
static int  PrintRET      (TNode *node);
static int  PrintDEF      (TNode *node);
static int  PrintIN       (TNode *node);
static int  PrintOUT      (TNode *node);
static int  PrintNeg      (TNode *node);
static int  PrintWHILE    (TNode *node);
static int  PrintSERV     (TNode *node);
static int  PrintOP       (TNode *node);
static int  PrintAssn     (TNode *node);
static int  PrintConst    (TNode *node);
static int  PrintID       (TNode *node);
static int  PrintVar      (TNode *node);
static int  PrintSt       (TNode *node);
static int  PrintUNARY    (TNode *node);
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

#define SHR(tgt, num) PrintA ("shr %s, %d ; pseudo-float emul", tgt, num);
#define SHL(tgt, num) PrintA ("shl %s, %d ; pseudo-float emul", tgt, num);

#define NUMS_AFTER_POINT 9

#define FLOAT_R(tgt) SHR (tgt, NUMS_AFTER_POINT)
#define FLOAT_L(tgt) SHL (tgt, NUMS_AFTER_POINT)

#define INC(trgt) PrintA ("inc %s", trgt);
#define PUSH(src) PrintA ("push %s", src);
#define POP(dst)  PrintA ("pop %s", dst);

#define SAVE() PrintA ("pop %s\n", RES)

#endif

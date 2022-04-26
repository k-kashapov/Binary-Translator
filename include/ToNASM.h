#ifndef TONASM_H
#define TONASM_H

#include "Lang.h"

static FILE *AsmFile   = NULL;
static int  IdsNum     = 0;
static Id   *IdsArr    = NULL;
static int  Curr_rsp   = 0;
static int  MemOffset  = 0;
static int  Tabs       = 0;
static Id   *GlobalArr = NULL;
static int  GlobalNum  = 0;

static void PrintA        (const char *msg, ...);
static int  PrintCallArgs (TNode *node);
static int  PrintCALL     (TNode *node);
static int  PrintRET      (TNode *node);
static int  PrintDEF      (TNode *node);
static int  PrintIN       (TNode *node);
static int  PrintOUT      (TNode *node);
static int  PrintNeg      (TNode *node);
static int  PrintIF       (TNode *node);
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

#define ASM_IDS    &IDS,       &IDNUM
#define GLOBAL_IDS &GlobalArr, &GlobalNum

// NOT IMPLEMENTED YET:
// GeneralRegs are used to store variables. The order they are written in
// is essential, because this the order they will be assigned.
// This way we try to reduce the amount of memory usage: rdi is used a lot
// to pass arguments

static const int  VarRegsNum     = 12;
static const int  MaxArgsRegs    = 6;
static const char *GeneralRegs[] = { "rbx", "r10", "r11", "r12", "r13",
                                     "r14", "r15", "r9",  "r8",  "rcx",
                                     "rdx", "rsi", "rdi" };

#define RES  "rax" // ret value
#define MEM  "rbp"
#define FREE "rsp"

#define ADD_SS(dst, src) PrintA ("add %s, %s", dst, src); // args: str, str
#define ADD_SD(dst, src) PrintA ("add %s, %d", dst, src); // args: str, num
#define MOV_SS(dst, src) PrintA ("mov %s, %s", dst, src);

#define INC(trgt) PrintA ("inc %s", trgt);
#define PUSH(src) PrintA ("push %s", src);
#define POP(dst)  PrintA ("pop %s", dst);

#define SAVE() PrintA ("pop %s\n", RES)

#endif

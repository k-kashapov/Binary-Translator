#ifndef TONASM_H
#define TONASM_H

#include "Lang.h"

static FILE *AsmFile   = NULL;
static int  IdsNum     = 0;
static Id   *IdsArr    = NULL;
static int  FreeOffset = 0;
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

static const char *ArgumentRegs[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };
static const int  MaxArgsRegs  = 6;

#define RES  "rax"
#define MEM  "rbp"
#define FREE "rsp"

#define ADD(dst, src) PrintA ("add " # dst ", " # src);
#define MOV(dst, src) PrintA ("mov " # dst ", " # src);

#define INC(trgt) PrintA ("inc " # trgt);
#define PUSH(src) PrintA ("push " # src);
#define POP(dst)  PrintA ("pop "  # dst);

#define SAVE() PrintA ("pop %s\n", RES)

#endif

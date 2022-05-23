#include <stdio.h>
#include <assert.h>
#include <elf.h>
#include <Logs.h>
#include <stdlib.h>
#include <string.h>

#ifndef ELF_GEN_H
#define ELF_GEN_H 1

FILE *CreateTemplate (const char *name, int prog_len);
int  CloseEXE        (FILE *target, const char *name);

#endif

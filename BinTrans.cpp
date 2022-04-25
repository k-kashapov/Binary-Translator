#include <stdio.h>
#include <assert.h>
#include <elf.h>
#include <Logs.h>
#include <stdlib.h>
#include <string.h>

#define SET_IDENT(offs, value) dest->e_ident[offs] = value;
#define SET_FIELD(fld,  value) dest->fld           = value;

int *ConstructELFheader (Elf64_Ehdr *dest)
{
    assert (dest);

    LOG_MSG ("Creating a header for header <%p>\n", dest);

    SET_IDENT (EI_MAG0,       ELFMAG0);
    SET_IDENT (EI_MAG1,       ELFMAG1);
    SET_IDENT (EI_MAG2,       ELFMAG2);
    SET_IDENT (EI_MAG3,       ELFMAG3);
    SET_IDENT (EI_CLASS,      ELFCLASS64);
    SET_IDENT (EI_DATA,       ELFDATA2LSB);
    SET_IDENT (EI_VERSION,    EV_CURRENT);
    SET_IDENT (EI_OSABI,      ELFOSABI_SYSV);
    SET_IDENT (EI_ABIVERSION, 0);
    SET_IDENT (EI_PAD,        0);

    SET_FIELD (e_type,    ET_EXEC);
    SET_FIELD (e_machine, EM_X86_64);
    SET_FIELD (e_version, EV_CURRENT);
    SET_FIELD (e_entry,   0x401000);
    SET_FIELD (e_phoff,   64);

    // SET_FIELD (e_shoff, 00);
    // SET_FIELD (e_flags, 00);

    SET_FIELD (e_ehsize,    64);
    // SET_FIELD (e_phentsize, 00);

    return 0;
}

// Maximum name length for executable = 16

int CloseEXE (FILE *target, const char *name)
{
    assert (target);
    assert (name);

    fclose (target);

    char command[9 + 16] = "chmod +x ";

    strncat (command + 9, name, 16);
    system (command);

    return 0;
}

int main (int argc, const char **argv)
{
    FILE *tmp = fopen ("test", "wb");

    CloseEXE (tmp, "test");
    
    return 0;
}


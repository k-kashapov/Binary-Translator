#include "ELF_Gen.h"

// Special thanks to Varnike and EnikAs on github for helping with
// BYTECODE and ELF generation

#define SET_IDENT(offs, value) dest->e_ident[offs] = value;
#define SET_FIELD(fld,  value) dest->fld           = value;

static int *ConstructELFheader (Elf64_Ehdr *dest)
{
    assert (dest);

    LOG_MSG ("Creating ELF header <%p>\n", dest);

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

    SET_FIELD (e_type,    ET_DYN);
    SET_FIELD (e_machine, EM_X86_64);
    SET_FIELD (e_version, EV_CURRENT);
    SET_FIELD (e_entry,   0x8048080);
    SET_FIELD (e_phoff,   64);

    SET_FIELD (e_shoff, 00);
    SET_FIELD (e_flags, 00);

    SET_FIELD (e_ehsize,    64);
    SET_FIELD (e_phentsize, sizeof (Elf64_Phdr));
    SET_FIELD (e_phnum,     01);

    SET_FIELD (e_shentsize, 00);
    SET_FIELD (e_shnum,     00);
    SET_FIELD (e_shstrndx,  00);

    return 0;
}

static const int F_READ  = 0x04,
                 F_WRITE = 0x02,
                 F_EXEC  = 0x01;

static int *ConstructPROGheader (Elf64_Phdr *dest, int prog_len)
{
    assert (dest);

    LOG_MSG ("Creating a program header <%p>\n", dest);

    SET_FIELD (p_type,    PT_LOAD);
    SET_FIELD (p_offset,  0x78);
    SET_FIELD (p_vaddr,   0x8048080);
    SET_FIELD (p_paddr,   0);
    SET_FIELD (p_filesz,  prog_len);
    SET_FIELD (p_memsz,   prog_len);
    SET_FIELD (p_flags,   F_READ + F_WRITE + F_EXEC);
    SET_FIELD (p_align,   0x1000);

    return 0;
}

// Maximum name length for executable = 16

int CloseEXE (FILE *target, const char *name)
{
    assert (target);
    assert (name);

    fclose (target);

    char command[9 + 20] = "chmod +x ";

    strncat (command, name, 16);

    return system (command);
}

FILE *CreateTemplate (const char *name, int prog_len)
{
    FILE *templ = fopen (name, "wb");

    Elf64_Ehdr hdr = {};

    ConstructELFheader (&hdr);
    fwrite (&hdr, sizeof (hdr), 1, templ);

    LOG_MSG ("written ELF header to file, bytes: %ld\n", ftell (templ));

    Elf64_Phdr pHdr = {};

    ConstructPROGheader (&pHdr, prog_len);
    fwrite (&pHdr, sizeof (pHdr), 1, templ);

    LOG_MSG ("written Program header to file, bytes: %ld\n", ftell (templ));

    return templ;
}

#include <stdio.h>
#include <assert.h>

#ifdef IS_LINUX
#include <stdlib.h>
#endif

int ConstructELFTemplate (FILE *dest)
{
    assert (dest);

    // Constant part of header                                   little
    //                             Magic constant       64-bit   endian   IDK    System-V
    //                           /                 \     /  \     /  \    /  \  /        \.
    const char header_const[] = { 0x7F, 'E', 'L', 'F',   0x02,    0x01,   0x01, 0x00, 0x00,

    //                                                                   AMD
    //                                Dummy bytes          EXE          x86-64
    //                            /                 \   /        \    /        \.
                                  0, 0, 0, 0, 0, 0, 0,  0x02, 0x00,   0x3E, 0x00,

    //                            Orig ver of ELF
    //                            /           \.
                                  0x01, 0, 0, 0,

    //                                  Entry point
    //                            /                    \.
                                  0, 0, 0, 0, 0, 0, 0, 0,

    //                            Start of the header table
    //                            /                       \.
                                  0x40, 0, 0, 0, 0, 0, 0, 0,

    //                               Start of the section
    //                                   header table         Flags      Header size
    //                            /                    \    /        \    /        \.
                                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0,   0x40, 0x00,

    //                            Header table    Num      Section header       Num of hdr
    //                            entry size   of entries  table entry size     tbl entries
    //                              /  \         /  \          /  \               /  \.
                                    0, 0,        0, 0,         0, 0,              0, 0, 

    //                           Idx of the sctn
    //                           hdr tbl entry with
    //                           section names
    //                              /  \.
                                    0, 0 };

    fwrite (header_const, sizeof (char), sizeof (header_const), dest);


    return 0;
}

int main (int argc, const char **argv)
{
    FILE *temp = fopen (
                        #ifdef IS_LINUX
                                "b.out",
                        #else
                                "b.exe",
                        #endif
                        "wb");

    ConstructELFTemplate (temp);

    fclose (temp);
    #ifdef IS_LINUX
    system ("chmod +x b.out");
    #endif

    return 0;
}


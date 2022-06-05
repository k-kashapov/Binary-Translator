# Binary-Translator
Translator for my language backend that creates x86_64 code.

The language itself is [one of my previous projects](https://github.com/k-kashapov/lang). The grammar is based on My Favourite Anecdotes.

# Usage

Use ```$ make lang_no_misc``` to compile version without miscellanious additions (producing tree image and saving tree to file).

## Flags
1) ```-i``` specify input file
2) ```-o``` specify output file
3) ```-S``` to compile into NASM code
4) ```-B``` to compile into an x86_64 ELF executable
5) if no flags other than ```-i``` and ```-o``` used, the code is compiled into [my Assembly language](https://github.com/k-kashapov/processor).

Current version does not support ```-i``` when using ```-B```. ELF output will always be called ```anek.exe```.

## Grammar

    G     ::= Купил мужик шляпу. OP+ А она ему как раз, господа.

    OP    ::= Dec || Func || IF || While || Call || Assn || Ret || Print.

    Dec   ::= "ID [всегда N]?" - подумал Штирлиц N раз. // ID is var name. Всегда N is equivalent to const ID = N. N раз is array of length N.

    Func  ::= Господа, а не сыграть ли нам в новую игру.
              ID называется. Правила очень просты: ID+. // First ID is function name. Then parameters
              Алга
                  OP*
              Развернулся и алга.

    IF    ::= Кто прочитал E тот сдохнет. // Equivalent to if (E)
                  OP*
              Ставь лайк
                  OP*
              и можешь считать, что не читал.

    While ::= В дверь постучали E раз. // Equivalent to while (E)
                  OP*
              Дверь отвалилась.

    Call  ::= Анекдот: Заходят как-то в бар ID+. // Function parameters
              А бармен им говорит: ID. // Function name

    Assn  ::= Этим ID был E.

    Ret   ::= Козырная E, господа.

    E     ::= Cmp ([|| &&] Cmp)*

    Cmp   ::= Sum ([> < <= >= == !=] Sum)*

    Sum   ::= T ([+-] T)*

    T     ::= Pow ([дофига /] Pow)*

    Pow   ::= Neg (^ Neg)*

    Neg   ::= Нифига? P

    P     ::= Биба E Боба || Call || Scan || N

    Scan  ::= ввод пользователем числового значения с клавиатуры

    Print ::= Голос, ID!

    ID    ::= [a-zA-Z]+

# Implementation details

* [ELF-file generation](#elf-file-generation)
* [File structure](#file-structure)
* [Code structure](#code-structure)
* [Code generation](#code-generation)

## ELF-file generation

The program generates the most basic executable ELF-file.

ELF-file generation is implemented in [ELF_Gen.cpp](/src/ELF_Gen.cpp) and [ToBIN.cpp](/src/ToBIN.cpp).

Virtual load address will always be equal to ```<0x08048080>```.

Code length should not exceed ```$PAGESIZE```
environment variable value. In this paper we consider ```$PAGESIZE``` to be equal to ```0x1000```.

These limitations will be fixed in the future patches.

## File structure

Table of ELF-file contents

|    Section     | Offset |      Length     |                                                              Content                                                             |
|:--------------:|:------:|:---------------:|:--------------------------------------------------------------------------------------------------------------------------------:|
|   ELF header   |    0   |       0x40      |                                                         Basic ELF-header.                                                        |
| Program header |  0x40  |       0x38      |                  Basic program header. The only part that is changed in template is the executable code length.                  |
|      Code      |  0x78  | [0x1AA; 0x11AA] | Executable code. Minimum size is 0x1AA bytes due to precompiled PRINT, SCAN and POW functions being forcefully pasted into file. |

## Code structure

Table of Code contents. Offsets are relative to the executable code section start (0x78).

|      Section      | Offset |    Length   |                          Content                          |
|:-----------------:|:------:|:-----------:|:---------------------------------------------------------:|
|    ```pusha```    |    0   |     0x0A    |               Push all calee-saved registers              |
|  ```call main```  |  0x0A  |     0x05    |                  Call ```main()``` opcode                 |
|     ```popa```    |  0x0F  |     0x0A    |                  Pop all saved registers                  |
|  ```exit(rax)```  |  0x19  |     0x0A    | Exit the program, returning the value stored in ```rax``` |
|  stdlib: ```IN``` |  0x23  |     0x5F    |      Precompiled implementation of ```IN``` function      |
| stdlib: ```OUT``` |  0x82  |     0xD0    |      Precompiled implementation of ```OUT``` function     |
| stdlib: ```POW``` |  0x152 |     0x58    |      Precompiled implementation of ```POW``` function     |
|    Program code   |  0x1AA | [0; 0x1000] |                    Actual program code                    |

## Code generation

Program text written in the anek language is translated into a binary tree.

<img src = "https://user-images.githubusercontent.com/52855633/172031042-f548e92d-cb1e-4197-8e7a-dd2e645ce947.png" width = 50%>

Binary code is generated by traversing the tree.

Example:

```
static int PrintID (TNode *node)
{
    PrintA ("; %.*s", LEN, DECL);

    if (LEFT)
    {
        $ int lErr = NodeToAsm (LEFT);
        if (lErr) return lErr;
    }

    if (RIGHT)
    {
        $ int rErr = NodeToAsm (RIGHT);
        if (rErr) return rErr;
    }

    return 0;
}
```

# Performance test

The main goal of this project was to increase the execution speed of programs written in our language.

To check the effect, we have created a simple program that calculates the factorial of 12.

We then run the program 100 000 times and check time of execution using Linux ```time``` tool.

The ELF execution time is then compared with the same task performance on our Processor.

| Architecture | Exec. Time, s |
|:------------:|:-------------:|
|   Processor  |  5.20 ± 0.10  |
|    x86_64    | 0.011 ± 0.001 |

#### Overall speedup is approximately 450x

# Acknowledgements

I would like to express my gratitude to [Varnike](https://github.com/Varnike), [EnikAs](https://github.com/EnikAs) and [deGekata](https://github.com/deGekata) for helping with opcode and ELF generation.

Additional thanks to [Rustam](https://github.com/RustamSubkhankulov) for reading this README.

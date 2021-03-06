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

<details>

<summary>Expand me</summary>

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

</details>

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

<details>

<summary>Example</summary>

<img src = "https://user-images.githubusercontent.com/52855633/172031042-f548e92d-cb1e-4197-8e7a-dd2e645ce947.png" width = 100%>

</details>

Binary code is generated by traversing the tree.

<details>

<summary>Example</summary>

```
static int PrintID (TNode *node)
{
    // Print to Listing file
    
    PrintA ("; %.*s", LEN, DECL);

    // Run translation recursively from left and right children

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
</details>

# Command generation details

For each command used in the resulting program there's a defined INSTRUCTION structure in file
```include/NASM_BIN_TABLE.h```.

The structure contains opcode itself, opcode length, argument
and argument length.

For example:

```
//      MNEMO                                     OPCODE     LEN  ARG_LEN   ARG
#define SUB_RSP_1_BYTE(arg_)       (INSTRUCTION { 0xEC8348,    3,       1, arg_ })
```

This structure is then pasted into opcodes buffer.

### We will now give detailed explanations of each node type translation

* [General](#general)
* [Variables](#variables)
* [```CALL``` and ```RET```](#call-and-ret)
* [Function declaration](#function-declaration)
* [```IN``` and ```OUT```](#in-and-out)
* [```IF``` and ```WHILE```](#if-and-while)
* [Operators](#operators)

## General

* All variables are stored in stack. Space in stack between ```RBP``` and ```RSP```
  is the stack frame. 

* All operation results are stored in ```RAX```.

* All arithmetic operations support _pseudo-float_ calculations. We shift each integer value
  9 bits left. This gives us precision of around 0.002.

* All integers are signed.

## Variables

The language standard we use does not require variable declaration before usage. However, in the
language front-end implementation we treat undeclared variables as an error.


This results in a variable being initialized with zero, whenever it is seen for the 
first time in language tree.

When a variable is used in code, it's value is moved to ```RAX``` register.

## ```CALL``` and ```RET```

### ```CALL```

To call a function, we do the following steps:

1)  Put call arguments to stack.
    Contrast to FASTCALL and CDECL calling conventions, we push argument values relative to
    ```RSP``` with a negative offset.

<details>

<summary>Detailed stack</summary>

Let's say, we want to call a function ```int func (int first, int second)```. It has 2 parameters.

This is how stack looks like before calling:
                                                                                                                                                                                                    
|     Offset    |        0x0000       |  -0x0008   |   -0x0010  | -0x0018 | -0x0020 | -0x0028 | -0x0030 | -0x0038 |
|:-------------:|:-------------------:|:----------:|:----------:|:-------:|:-------:|:-------:|:-------:|:-------:|
| Stack example |        0x1234       |   0x0042   |   0x00CE   |  0x0000 |  0x0000 |  0x0000 |  0x0000 |  0x0000 |
|  Explanation  | Old ```RBP``` value | Variable A | Variable B | Garbage | Garbage | Garbage | Garbage | Garbage |
|   Registers   |      ```RBP```      |            |  ```RSP``` |         |         |         |         |         |

We then call ```func (A, B)``` in our language.

Function parameters will be placed into ```[RSP - 0x10 - 0x8 * N]```, where ```N``` is the
parameter number.

After all arguments are passed, the stack would look like this:

|     Offset    |        0x0000       |  -0x0008   |   -0x0010  | -0x0018 | -0x0020 |        -0x0028         |        -0x0030         | -0x0038 |
|:-------------:|:-------------------:|:----------:|:----------:|:-------:|:-------:|:----------------------:|:----------------------:|:-------:|
| Stack example |        0x1234       |   0x0042   |   0x00CE   |  0x0000 |  0x0000 |         0x0042         |         0x00CE         |  0x0000 |
|  Explanation  | Old ```RBP``` value | Variable A | Variable B | Garbage | Garbage | Parameter = Variable A | Parameter = Variable B | Garbage |
|   Registers   |      ```RBP```      |            |  ```RSP``` |         |         | ```RSP - 0x10 - 0x8``` | ```RSP - 0x10 - 0x10```|         |

</details>

2) Call the function: ```RIP``` is pushed to stack.

3) Create stack frame: ```RBP``` is pushed to stack and moved to the position of ```RSP```.

<details>

<summary>Stack after call</summary>

When the called function execution is started, the stack looks like this:

|     Offset    |        0x0000       |  -0x0008   |   -0x0010  |    -0x0018    |      -0x0020        |        -0x0028         |        -0x0030         | -0x0038 |
|:-------------:|:-------------------:|:----------:|:----------:|:-------------:|:-------------------:|:----------------------:|:----------------------:|:-------:|
| Stack example |        0x1234       |   0x0042   |   0x00CE   |     0xC0DE    |       0x0000        |         0x0042         |         0x00CE         |  0x0000 |
|  Explanation  | Old ```RBP``` value | Variable A | Variable B | Old ```RIP``` | Old ```RBP``` value | Parameter = Variable A | Parameter = Variable B | Garbage |
|   Registers   |                     |            |            |               | ```RBP```,```RSP``` |    ```RSP - 0x8```     |    ```RSP - 0x10```    |         |

</details>

This concludes the calling process.

### ```RET```

When returning from a function, all of the above actions are reverted:

1) ```mov RSP, RBP```

2) ```pop RBP```

3) ```ret```

## Function declaration

Whenever a function is declared, a stack frame creation code is pasted.
We then decrease ```RSP``` by ```0x8 * N```, where ```N``` - number of parameters this function
has. 

Then the function code is printed.

## ```IN``` and ```OUT```

### ```IN```

This function is responsible for reading an integer value from keyboard.
Although all computations use pseudo-floats, there's currently no way to read a floating point
value from keyboard.

The function is a part of my "stdlib" and is pasted into every file before the program code.

### ```OUT```

Prints a floating-point value to ```stdout```. The string is generated separately for the
integer and fractional parts of the value.

The function is a part of my "stdlib" and is pasted into every file before the program code.

## ```IF``` and ```WHILE```

Both nodes are processed almost identically, so we will give detailed info only on ```IF```.

When the translator encounters an ```IF``` node, the following actions are done:

1) Translate the condition node. It's value will be stored in ```RAX```

2) ```test RAX```

3) Print the conditional jump opcode (1 byte). Save current offset in opcodes buffer to variable (let's call it ```JmpArgPos```). Print dummy argument (4 bytes with value 0).

4) Translate the ```else``` branch

5) Save current position for future patching an uncoditional jump to the end of if statement.

6) Calculate the offset of current position from the conditional jump: ```OFFS = (Curr_offs - JmpArgPos - ArgLen)```

7) Patch the argument of the conditional jump, so it jumps at the start of the ```if true``` branch

8) Translate the ```if true``` branch

9) Patch the unconditional jump after the ```else``` branch

## Operators

Operators in our language can be split into several classes:

* [Assignment](#assingment)

* [Arithmetics](#arithmetics)

* [Comparisons](#comparisons)

* [Negation](#negation)

We will now give explanations for each class

### Assignment

To assign a value to a variable, we first translate the entire rValue node. It's result will
be stored in ```RAX```. We then ```mov [RBP - OFFS], RAX```, where ```OFFS``` is the variable
offset in stack.

For example, the following code:

```
"Количество" - подумал Штирлиц 1 раз.
Этим Количество был 2.
```

Assigns a constant value of 2 to the variable ```Количество```.


It will be translated into the following assembly code:

```
mov rax, 1024       ; const value << 9
mov [rbp - 24], rax ; Количество = rax
```

### Arithmetics

WIP

### Comparisons

WIP

### Negation

WIP

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

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

# Performance test

The main goal of this project was to increase the execution speed of our language.

To check the effect, we have created a simple program that calculates the factorial of 12.

We then run the program 100 000 times and check time of execution using Linux ```time``` tool.

The ELF execution time is then compared with the same task performance on our Processor.

| Architecture | Exec. Time, s |
|:------------:|:-------------:|
|   Processor  |  18.50 ±0.37  |
|    x86_64    | 0.011 ± 0.001 |

#### Overall speedup is approximately 1700x

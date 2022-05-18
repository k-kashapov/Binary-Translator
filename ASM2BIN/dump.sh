nasm -f elf64 test.asm -o test
objdump -D -w -M intel test | grep "^ " | cut -f2,3 --output-delimiter=", \"" | sed 's/\(,$\)/\"\1\"/' | sed 's/^/#define BIN (/' | sed 's/$/\")/' > objdump.h
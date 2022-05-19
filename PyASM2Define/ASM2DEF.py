src = open ("source.txt", "r")

for line in src:
    line_split = line.replace(',', ' ').split()
    if len (line_split) < 1:
        continue

    line_full = line_split[0] + " "

    is_name   = 0
    is_opcode = 0

    opcode = []

    for word in line_split:

        if is_name and word[0].isalpha():
            line_full += word.upper() + "_"

        elif word[0].isnumeric() and not is_name:
            is_opcode = 1

        if word[0] == '|':
            is_name   = 1
            is_opcode = 0

        if is_opcode:
            opcode.append (word.upper())

    opcode.reverse()
    opcode[-1] = opcode[-1][1:]

    define_op = "\t\t (INSTRUCTION { 0x" + "".join(opcode) + ","

    print (line_full[:-1], define_op, str (len (opcode)) + ", 0,", "arg_ })")

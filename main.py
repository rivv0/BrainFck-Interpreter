def compile_to_bytecode(code):
    bytecode = []
    loop_stack = []
    pc = 0

    while pc < len(code):
        command = code[pc]

        if command in ['>', '<', '+', '-']:
            count = 1
            while pc + count < len(code) and code[pc + count] == command:
                count += 1
            bytecode.append((command, count))
            pc += count - 1
        elif command == '.':
            bytecode.append(('.',))
        elif command == ',':
            bytecode.append((',',))
        elif command == '[':
            if pc + 2 < len(code) and code[pc + 1] == '-' and code[pc + 2] == ']':
                bytecode.append(('[-]',))
                pc += 2
            else:
                bytecode.append(('[', len(bytecode)))
                loop_stack.append(len(bytecode) - 1)
        elif command == ']':
            if not loop_stack:
                raise SyntaxError("Unmatched loop end")
            start = loop_stack.pop()
            bytecode.append((']', start))
            bytecode[start] = ('[', len(bytecode) - 1)

        pc += 1

    if loop_stack:
        raise SyntaxError("Unmatched loop start")

    return bytecode

def execute_bytecode(bytecode):
    memory = [0] * 30000
    ptr = 0
    pc = 0
    output = []

    while pc < len(bytecode):
        instruction, *args = bytecode[pc]

        if instruction == '>':
            ptr += args[0]
        elif instruction == '<':
            ptr -= args[0]
        elif instruction == '+':
            memory[ptr] = (memory[ptr] + args[0]) % 256
        elif instruction == '-':
            memory[ptr] = (memory[ptr] - args[0]) % 256
        elif instruction == '.':
            output.append(chr(memory[ptr]))
        elif instruction == ',':
            memory[ptr] = ord(input()[0])
        elif instruction == '[-]':
            memory[ptr] = 0
        elif instruction == '[':
            if memory[ptr] == 0:
                pc = args[0]
        elif instruction == ']':
            if memory[ptr] != 0:
                pc = args[0]

        pc += 1

    print(''.join(output))

def repl():
    while True:
        try:
            code = input("CCBF> ")
            bytecode = compile_to_bytecode(code)
            execute_bytecode(bytecode)
        except EOFError:
            break

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 1:
        repl()
    else:
        with open(sys.argv[1], 'r') as file:
            code = file.read()
            bytecode = compile_to_bytecode(code)
            execute_bytecode(bytecode)
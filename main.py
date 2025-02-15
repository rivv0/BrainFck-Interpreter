def repl():
    while True:
        try:
            code = input("CCBF> ")
            print(code)
        except EOFError:
            break

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 1:
        repl()
    else:
        with open(sys.argv[1], 'r') as file:
            code = file.read()
            print(code)
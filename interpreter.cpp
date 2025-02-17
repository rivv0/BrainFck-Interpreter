#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>

struct Instruction {
    char command;
    int argument;
};

std::vector<Instruction> compile_to_bytecode(const std::string& code) {
    std::vector<Instruction> bytecode;
    std::stack<int> loop_stack;
    std::unordered_map<int, int> loop_map;
    
    for (size_t pc = 0; pc < code.size(); ++pc) {
        char command = code[pc];
        
        if (command == '>' || command == '<' || command == '+' || command == '-') {
            int count = 1;
            while (pc + 1 < code.size() && code[pc + 1] == command) {
                ++count;
                ++pc;
            }
            bytecode.push_back({command, count});
        } else if (command == '.' || command == ',') {
            bytecode.push_back({command, 0});
        } else if (command == '[') {
            int start = bytecode.size();
            bytecode.push_back({'[', 0});
            loop_stack.push(start);
        } else if (command == ']') {
            if (loop_stack.empty()) {
                throw std::runtime_error("Unmatched loop end");
            }
            int start = loop_stack.top();
            loop_stack.pop();
            int end = bytecode.size();
            bytecode.push_back({']', start});
            bytecode[start].argument = end;
        }
    }
    
    if (!loop_stack.empty()) {
        throw std::runtime_error("Unmatched loop start");
    }
    
    return bytecode;
}

void execute_bytecode(const std::vector<Instruction>& bytecode) {
    std::vector<unsigned char> memory(30000, 0);
    size_t ptr = 0;
    size_t pc = 0;
    
    while (pc < bytecode.size()) {
        const auto& instr = bytecode[pc];
        
        switch (instr.command) {
            case '>': ptr += instr.argument; break;
            case '<': ptr -= instr.argument; break;
            case '+': memory[ptr] += instr.argument; break;
            case '-': memory[ptr] -= instr.argument; break;
            case '.': std::cout << static_cast<char>(memory[ptr]); break;
            case ',': memory[ptr] = std::cin.get(); break;
            case '[': if (memory[ptr] == 0) pc = instr.argument; break;
            case ']': if (memory[ptr] != 0) pc = instr.argument; break;
        }
        ++pc;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::string code;
        std::cout << "CCBF> ";
        std::getline(std::cin, code);
        auto bytecode = compile_to_bytecode(code);
        execute_bytecode(bytecode);
    } else {
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Error opening file: " << argv[1] << '\n';
            return 1;
        }
        std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        auto bytecode = compile_to_bytecode(code);
        execute_bytecode(bytecode);
    }
    return 0;
}

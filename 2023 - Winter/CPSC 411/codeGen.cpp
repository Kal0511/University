#include <iostream>

#include <set>
#include <cstring>

#include "AST.h"

#include "codeGen.h"

// stores all used registers on the stack
void CodeGenerator::storeStack() {
    auto used = stack.back().usedRegisters;
    int stackLevel = 0;
    if (!used.empty()) {
        writeLine("\taddi $sp, $sp, -" + to_string(4 * used.size()));
    }
    for (const auto &i: used) {
        writeLine("\tsw " + i + ", " + std::to_string(stackLevel) + "($sp)");
        stackLevel += 4;
    }

    stack.emplace_back();
}

// restores all used registers on the stack
void CodeGenerator::restoreStack() {
    stack.pop_back();
    auto used = stack.back().usedRegisters;
    size_t stackLevel = 4 * used.size();
    for (long i = (long) used.size() - 1; i >= 0; i--) {
        stackLevel -= 4;
        writeLine("\tlw " + used[i] + ", " + std::to_string(stackLevel) + "($sp)");
    }
    if (!used.empty()) {
        writeLine("\taddi $sp, $sp, " + to_string(4 * used.size()));
    }

}

// stores all function parameter registers on the stack
void CodeGenerator::storeParaStack() {
    auto paraReg = stack.back().idToParaReg;
    int stackLevel = 0;
    if (!paraReg.empty()) {
        writeLine("\taddi $sp, $sp, -" + to_string(4 * paraReg.size()));
    }
    for (const auto &reg: paraReg) {
        writeLine("\tsw " + reg.second + ", " + std::to_string(stackLevel) + "($sp)");
        stackLevel += 4;
    }
}

// restores all function parameter registers on the stack
void CodeGenerator::restoreParaStack() {

    auto paraReg = stack.back().idToParaReg;
    int stackLevel = 0;
    for (const auto &reg: paraReg) {
        writeLine("\tlw " + reg.second + ", " + std::to_string(stackLevel) + "($sp)");
        stackLevel += 4;
    }
    if (!paraReg.empty()) {
        writeLine("\taddi $sp, $sp, " + to_string(4 * paraReg.size()));
    }
}

// used to allocate registers for operations
string CodeGenerator::allocReg() {
    if (stack.back().freeRegisters.empty()) {
        cerr << "Error: expression too complicated" << endl;
        exit(1);
    }

    string last = stack.back().freeRegisters.back();
    stack.back().usedRegisters.push_back(last);
    stack.back().freeRegisters.pop_back();
    return last;
}

// used to free registers once done using
void CodeGenerator::freeReg(const string &reg) {
    if (reg[1] != 'a') {
        stack.back().freeRegisters.push_back(reg);
        for (auto iter = stack.back().usedRegisters.begin(); iter != stack.back().usedRegisters.end(); ++iter) {
            if (*iter == reg) {
                stack.back().usedRegisters.erase(iter);
                break;
            }
        }
    }
}

// used to map id names to registers
void CodeGenerator::mapReg(const string &id, const string &reg) {
    stack.back().idToParaReg[id] = reg;
}

// frees mapped ids and registers
void CodeGenerator::unmapReg(const string &id) {
    stack.back().idToParaReg.erase(id);
}

// checks if register is allocated for id
bool CodeGenerator::checkReg(const string &id) {
    return stack.back().idToParaReg.find(id) != stack.back().idToParaReg.end();
}

// returns register allocated to id
string CodeGenerator::getReg(const string &id) {
    return stack.back().idToParaReg.at(id);
}

// transforms string in to mips assembly code
void CodeGenerator::writeString(char *curr) {
    writeLine("S" + std::to_string(stringCount) + ":");
    while (*curr) {
        if (*curr == '\\' && *(curr + 1)) {
            switch (*(curr + 1)) {
                case 'n':
                    writeLine("\t.byte " + std::to_string(int('\n')));
                    break;
                case 'b':
                    writeLine("\t.byte " + std::to_string(int('\b')));
                    break;
                case 'f':
                    writeLine("\t.byte " + std::to_string(int('\f')));
                    break;
                case 't':
                    writeLine("\t.byte " + std::to_string(int('\t')));
                    break;
                case 'r':
                    writeLine("\t.byte " + std::to_string(int('\r')));
                    break;
                case '\'':
                    writeLine("\t.byte " + std::to_string(int('\'')));
                    break;
                case '\"':
                    writeLine("\t.byte " + std::to_string(int('\"')));
                    break;
                case '\\':
                    writeLine("\t.byte " + std::to_string(int('\\')));
                    break;
            }
            curr++;
        } else {
            writeLine("\t.byte " + std::to_string(int(*curr)));
        }
        curr++;
    }
    writeLine("\t.byte 0");
}

void CodeGenerator::preCB1(AST::ASTNode *node) {
    if (auto decl = dynamic_cast<AST::GlobalDeclaration *>(node)) {
        if (std::strcmp(decl->variable->variableType, "string") == 0) {
            writeLine("val" + std::string(decl->variable->variableName) + to_string(decl->record->num) + ":\t.word S");
        } else {
            writeLine("val" + std::string(decl->variable->variableName) + to_string(decl->record->num) + ":\t.word 0");
        }
    }
    if (dynamic_cast<AST::Declaration *>(node)) {
    }
    if (auto primary = dynamic_cast<AST::Literal *>(node)) {
        if (strcmp(primary->type, "string") == 0) {
            primary->num = stringCount;
            writeString(const_cast<char *>(primary->data));
            stringCount++;
        }
    }
    if (auto func = dynamic_cast<AST::Function *>(node)) {
        definedFunctions.insert(func->functionName);
    }
}

void CodeGenerator::preCB2(AST::ASTNode *node) {
    if (dynamic_cast<AST::Block *>(node)) {
    }
    if (auto decl = dynamic_cast<AST::Declaration *>(node)) {
        currDecelerationCount += 1;
        decl->record->num = currDecelerationCount;
        if (strcmp(decl->variable->variableType, "string") == 0) {
            writeLine("\tla $a3, S");
            writeLine("\tsw $a3, " + to_string(4 * (toalDecelerationCount - decl->record->num)) + "($sp)");
        } else {
            writeLine("\tsw $zero, " + to_string(4 * (toalDecelerationCount - decl->record->num)) + "($sp)");
        }
    }
    if (auto func = dynamic_cast<AST::Function *>(node)) {
        currDecelerationCount = 0;
        toalDecelerationCount = func->num;
        stack = {Frame()};
        writeLine(std::string(func->functionName) + ":");
        if (std::string(func->functionName) != "main") {
            writeLine("\taddi $sp, $sp, -4");
            writeLine("\tsw $ra, ($sp)");
        }

        writeLine("\taddi $sp, $sp, -" + to_string(4 * toalDecelerationCount));

        if (func->signature->parameters) {
            int i = 0;
            for (auto parameter: *func->signature->parameters) {
                mapReg(parameter->parameterName, "$a" + to_string(i));
                i++;
            }
        }
    }
    if (auto For = dynamic_cast<AST::For *>(node)) {
        whileCount++;
        whileStack.push_back(whileCount);
        writeLine("whileStart" + std::to_string(whileCount) + ":");
        prePostOrder(For->condition);
        writeLine("\tbeq $zero, " + For->condition->reg + ", whileEnd" + std::to_string(whileCount));
        freeReg(For->condition->reg);
        For->children.erase(For->children.begin());
    }
    if (auto IF = dynamic_cast<AST::If *>(node)) {
        ifCount++;
        int currIfcount = ifCount;
        writeLine("ifStart" + std::to_string(currIfcount) + ":");
        prePostOrder(IF->condition);
        if (IF->falseBlock == nullptr) {
            writeLine("\tbeq $zero, " + IF->condition->reg + ", ifEnd" + std::to_string(currIfcount));
        } else {
            writeLine("\tbeq $zero, " + IF->condition->reg + ", ifElse" + std::to_string(currIfcount));
        }

        freeReg(IF->condition->reg);

        prePostOrder(IF->trueBlock);
        if (IF->falseBlock != nullptr) {
            writeLine("\tj ifEnd" + std::to_string(currIfcount));
            writeLine("ifElse" + std::to_string(currIfcount) + ":");
            prePostOrder(IF->falseBlock);
        }

        writeLine("ifEnd" + std::to_string(currIfcount) + ":");

        IF->children.clear();
    }
    if (auto literal = dynamic_cast<AST::Literal *>(node)) {
        if (strcmp(literal->type, "string") == 0) {
            node->reg = allocReg();
            writeLine("\tla " + node->reg + ", S" + std::to_string(literal->num));
        }
        if (strcmp(literal->type, "int") == 0) {
            node->reg = allocReg();
            writeLine("\tli " + node->reg + ", " + literal->data);
        }
    }
    if (auto assign = dynamic_cast<AST::Assignment *>(node)) {
        prePostOrder(assign->right);
        auto primary = dynamic_cast<AST::PrimaryExpression *>(assign->left);
        auto operand = dynamic_cast<AST::OperandName *>(primary->operation);


        if (operand->record->isGlobal) {
            writeLine("\tsw " + assign->right->reg + ", val" + std::string(operand->data) + to_string(operand->num));
        } else {
            writeLine("\tsw " + assign->right->reg + ", " + to_string(4 * (toalDecelerationCount - operand->record->num)) + "($sp)");
        }
        assign->children.clear();
        freeReg(assign->right->reg);
    }

    if (auto binary = dynamic_cast<AST::BinaryExpression *>(node)) {
        int currBinaryCount = binaryCount;
        binaryCount++;
        if (string(binary->symbol) == "&&") {
            binary->reg = allocReg();
            prePostOrder(binary->left);
            writeLine("\tbeq $zero, " + binary->left->reg + ", binarySkip" + std::to_string(currBinaryCount));
            prePostOrder(binary->right);
            string operation = binaryMap.at(binary->symbol);
            writeLine("\t" + operation + " " + binary->reg + ", " + binary->left->reg + ", " + binary->right->reg);
            writeLine("\t j binaryEnd" + std::to_string(currBinaryCount));
            writeLine("binarySkip" + std::to_string(currBinaryCount) + ":");
            writeLine("\tmove " + binary->reg + ", " + binary->left->reg);
            writeLine("binaryEnd" + std::to_string(currBinaryCount) + ":");
            freeReg(binary->left->reg);
            freeReg(binary->right->reg);
            binary->children.clear();
        }
        if (string(binary->symbol) == "||") {
            binary->reg = allocReg();
            prePostOrder(binary->left);
            writeLine("\tbne $zero, " + binary->left->reg + ", binarySkip" + std::to_string(currBinaryCount));
            prePostOrder(binary->right);
            string operation = binaryMap.at(binary->symbol);
            writeLine("\t" + operation + " " + binary->reg + ", " + binary->left->reg + ", " + binary->right->reg);
            writeLine("\t j binaryEnd" + std::to_string(currBinaryCount));
            writeLine("binarySkip" + std::to_string(currBinaryCount) + ":");
            writeLine("\tmove " + binary->reg + ", " + binary->left->reg);
            writeLine("binaryEnd" + std::to_string(currBinaryCount) + ":");
            freeReg(binary->left->reg);
            freeReg(binary->right->reg);
            binary->children.clear();
        }
    }

}

void CodeGenerator::postCB2(AST::ASTNode *node) {
    if (dynamic_cast<AST::Block *>(node)) {
    }
    if (auto unary = dynamic_cast<AST::UnaryExpression *>(node)) {
        unary->reg = allocReg();
        if (*unary->symbol == '-') {
            writeLine("\tnegu " + unary->reg + ", " + unary->expression->reg);
        }
        if (*unary->symbol == '!') {
            writeLine("\txori " + unary->reg + ", " + unary->expression->reg + ", 1");
        }
        freeReg(unary->expression->reg);
    }
    if (auto func = dynamic_cast<AST::Function *>(node)) {

        writeLine("\taddi $sp, $sp, " + to_string(4 * (toalDecelerationCount)));

        if (strcmp(func->functionName, "main") == 0) {
            writeLine("\tli $v0, 10");
            writeLine("\tsyscall");
        } else {
            writeLine("\tlw $ra, 0($sp)");
            writeLine("\taddi $sp, $sp, 4");
            if (strcmp(func->signature->returnType, "void") != 0) {
                writeLine("\tla $a0, retErr");
                writeLine("\tjal _Gprints");
                writeLine("\tjal _Ghalt");
            }
            writeLine("\tjr $ra");
            if (func->signature->parameters) {
                for (auto parameter: *func->signature->parameters) {
                    unmapReg(parameter->parameterName);
                }
            }
        }
    }
    if (auto expr = dynamic_cast<AST::PrimaryExpression *>(node)) {
        if (auto primary = dynamic_cast<AST::OperandName *>(expr->operation)) {
            if (!expr->argumentsList.empty()) {
                const vector<AST::ASTNode *> arguments = *(expr->argumentsList[0]);
                storeParaStack();
                for (size_t i = 0; i < arguments.size(); i++) {
                    auto argumentExpr = dynamic_cast<AST::Expression *>(arguments[i]);
                    writeLine("\tmove $a" + std::to_string(i) + ", " + argumentExpr->reg);
                    freeReg(argumentExpr->reg);
                }
                storeStack();

                if (definedFunctions.find(primary->data) != definedFunctions.end()) {
                    writeLine("\tjal " + std::string(primary->data));
                } else {
                    writeLine("\tjal _G" + std::string(primary->data));
                }

                restoreStack();
                restoreParaStack();
                if (primary->returnType != "void") {
                    primary->reg = allocReg();
                    expr->reg = primary->reg;
                    writeLine("\tmove " + primary->reg + ", $v0");
                }
            } else {
                if (checkReg(primary->data)) {
                    primary->reg = getReg(primary->data);
                    expr->reg = primary->reg;
                    return;
                }

                primary->reg = allocReg();
                expr->reg = primary->reg;
                if (primary->record->type == "bool") {
                    if (strcmp(primary->data, "true") == 0) {
                        writeLine("\tli " + primary->reg + ", 1");
                        return;
                    }
                    if (strcmp(primary->data, "false") == 0) {
                        writeLine("\tli " + primary->reg + ", 0");
                        return;
                    }
                }

                if (primary->record->isGlobal) {
                    writeLine("\tlw " + primary->reg + ", val" + std::string(primary->data) + to_string(primary->num));
                } else {
                    writeLine("\tlw " + primary->reg + ", " + to_string(4 * (toalDecelerationCount - primary->record->num)) + "($sp)");
                }
            }
        }
    }
    if (dynamic_cast<AST::For *>(node)) {
        int currWhileCount = whileStack.back();
        whileStack.pop_back();
        writeLine("\tj whileStart" + std::to_string(currWhileCount));
        writeLine("whileEnd" + std::to_string(currWhileCount) + ":");
    }
    if (dynamic_cast<AST::Break *>(node)) {

        int currWhileCount = whileStack.back();
        writeLine("\tj whileEnd" + std::to_string(currWhileCount));
    }
    if (auto primary = dynamic_cast<AST::PrimaryExpression *>(node)) {
        primary->reg = primary->operation->reg;
    }
    if (auto binary = dynamic_cast<AST::BinaryExpression *>(node)) {
        if (binary->children.empty()) {
            return;
        }

        if (*binary->symbol == '/') {
            writeLine("\tbnez " + binary->right->reg + ", divOk" + std::to_string(divCount));
            writeLine("\tla $a0, divErr");
            writeLine("\tjal _Gprints");
            writeLine("\tjal _Ghalt");
            writeLine("divOk" + std::to_string(divCount) + ":");

            writeLine("\tli $t0, -2147483648");
            writeLine("\tbne $t0, " + binary->left->reg + ", divCheckEnd" + std::to_string(divCount));
            writeLine("\tli $t0, -1");
            writeLine("\tbne $t0, " + binary->right->reg + ", divCheckEnd" + std::to_string(divCount));
            writeLine("\tli $t0, 1");
            writeLine("\tmove " + binary->right->reg + ", $t0");
            writeLine("divCheckEnd" + std::to_string(divCount) + ":");

            divCount++;
        }

        string operation = binaryMap.at(binary->symbol);
        binary->reg = allocReg();
        writeLine("\t" + operation + " " + binary->reg + ", " + binary->left->reg + ", " + binary->right->reg);
        freeReg(binary->left->reg);
        freeReg(binary->right->reg);
    }
    if (auto ret = dynamic_cast<AST::Return *>(node)) {
        writeLine("\taddi $sp, $sp, " + to_string(4 * (toalDecelerationCount)));

        if (ret->condition && ret->returnType != "void") {
            writeLine("\tmove $v0, " + ret->condition->reg);
            freeReg(ret->condition->reg);
        }
        writeLine("\tlw $ra, 0($sp)");
        writeLine("\taddi $sp, $sp, 4");
        writeLine("\tjr $ra");
    }
}

// generated base function for printing strings
std::string CodeGenerator::generate_prints() {
    std::string result = "_Gprints:\n";
    result += "\tmove $t0, $a0\n";
    result += "W1:\n ";
    result += "\tlbu $a0, ($t0)\n";
    result += "\tbeq $a0, $zero, EW1\n";
    result += "\tli $v0, 11\n";
    result += "\tsyscall\n";
    result += "\taddiu $t0, $t0, 1\n";
    result += "\tj W1\n";
    result += "EW1:\n";
    result += "\tjr $ra";
    return result;
}

// generated base function for printing chars
std::string CodeGenerator::generate_printc() {
    std::string result = "_Gprintc:\n";
    result += "\tli $v0, 11\n";
    result += "\tsyscall\n";
    result += "\tjr $ra";
    return result;
}

// generated base function for printing integers
std::string CodeGenerator::generate_printi() {
    std::string result = "_Gprinti:\n";
    result += "\tli $v0, 1\n";
    result += "\tsyscall\n";
    result += "\tjr $ra";
    return result;
}

// generated base function for printing booleans
std::string CodeGenerator::generate_printb() {
    std::string result = "_Gprintb:\n";
    result += "\taddi $sp, $sp, -4\n";
    result += "\tsw $ra, ($sp)\n";

    result += "\tbne $a0, $zero, PRINTTRUE\n";
    result += "\tla $a0, SFALSE\n";
    result += "\tjal _Gprints\n";
    result += "\tj PRINTEND\n";
    result += "PRINTTRUE:\n";
    result += "\tla $a0, STRUE\n";
    result += "\tjal _Gprints\n";
    result += "PRINTEND:\n";

    result += "\tlw $ra, 0($sp)\n";
    result += "\taddi $sp, $sp, 4\n";
    result += "\tjr $ra";
    return result;
}

// returns length of strings
std::string CodeGenerator::generate_len() {
    std::string result = "_Glen:\n";
    result += "\tli $v0, 0\n";
    result += "\tmove $t0, $a0\n";
    result += "W2:\n ";
    result += "\tlbu $a0, ($t0)\n";
    result += "\tbeq $a0, $zero, EW2\n";
    result += "\taddiu $t0, $t0, 1\n";
    result += "\taddiu $v0, $v0, 1\n";
    result += "\tj W2\n";
    result += "EW2:\n";
    result += "\tjr $ra";
    return result;
}

// stops program
std::string CodeGenerator::generate_halt() {
    std::string result = "_Ghalt:\n";
    result += "\tli $v0, 10\n";
    result += "\tsyscall";
    return result;
}

// requests for user input
std::string CodeGenerator::generate_getchar() {
    std::string result = "_Ggetchar:\n";
    result += "\tla $a0, GET\n";
    result += "\tli $a1, 2\n";
    result += "\tli $v0, 8\n";
    result += "\tsyscall\n";
    result += "\tlbu $v0, GET\n";
    result += "\tbne $v0, $zero, DONE\n";
    result += "\tli $v0, -1\n";
    result += "DONE:\n";
    result += "\tjr $ra";
    return result;
}

// generates mips assemble based in AST
CodeGenerator::CodeGenerator(AST::Program *program) {
    writeLine("\t.data");
    writeLine("S:");
    writeLine("\t.byte 0");
    writeLine("GET:");
    writeLine("\t.space 2");

    preOrder(program);
    writeLine("\t.align 2");
    writeLine("\t.text");

    writeLine("\t.globl main");
    prePostOrder(program);

    writeLine(generate_prints());
    writeLine(generate_printi());
    writeLine(generate_printc());
    writeLine(generate_printb());
    writeLine(generate_len());
    writeLine(generate_halt());
    writeLine(generate_getchar());

    writeLine("\t.data");
    writeLine("retErr:\t.byte 101, 114, 114, 111, 114, 58, 32, 102, 117, 110, 99, 116, 105, 111, 110, 32, 109, 117, 115, 116, 32, 114, 101, 116, 117, 114, 110, 32, 97, 32, 118, 97, 108, 117, 101, 10, 0");
    writeLine("\t.text");

    writeLine("\t.data");
    writeLine("STRUE:\t.byte 116, 114, 117, 101, 0");
    writeLine("\t.text");

    writeLine("\t.data");
    writeLine("SFALSE:\t.byte 102, 97, 108, 115, 101, 0");
    writeLine("\t.text");

    if (divCount > 0) {
        writeLine("\t.data");
        writeLine("divErr:\t.byte 101, 114, 114, 111, 114, 58, 32, 100, 105, 118, 105, 115, 105, 111, 110, 32, 98, 121, 32, 122, 101, 114, 111, 10, 0");
        writeLine("\t.text");
    }
}

void CodeGenerator::preOrder(AST::ASTNode *node) {
    preCB1(node);
    for (AST::ASTNode *child: node->children) {
        preOrder(child);
    }
}

void CodeGenerator::prePostOrder(AST::ASTNode *node) {
    preCB2(node);
    for (AST::ASTNode *child: node->children) {
        prePostOrder(child);
    }
    postCB2(node);
}


//
// Created by kal on 09/04/23.
//

#ifndef CPSC_411_CODEGEN_H
#define CPSC_411_CODEGEN_H

#include <iostream>
#include <utility>
#include <set>

#include "AST.h"

// used to generate mips assembly code using the AST
class CodeGenerator {
public:
    class Frame {
    public:
        vector <string> usedRegisters;
        vector <string> freeRegisters;
        unordered_map <string, string> idToParaReg;

        Frame() : usedRegisters(), freeRegisters({"$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"}) {}
    };

    vector <Frame> stack = {Frame()};

    void storeStack();

    void restoreStack();

    void storeParaStack();

    void restoreParaStack();


    string allocReg();

    void freeReg(const string &reg);


    void mapReg(const string &id, const string &reg);

    void unmapReg(const string &id);

    bool checkReg(const string &id);

    string getReg(const string &id);


    set <string> definedFunctions;
    int stringCount = 0;
    int whileCount = 0;
    vector<int> whileStack;
    int ifCount = 0;
    vector<int> ifStack;
    int divCount = 0;
    int toalDecelerationCount = 0;
    int currDecelerationCount = 0;
    int binaryCount = 0;

    unordered_map <string, string> binaryMap = {
            make_pair("==", "seq"),
            make_pair("!=", "sne"),

            make_pair("&&", "and"),
            make_pair("||", "or"),

            make_pair("<", "slt"),
            make_pair(">", "sgt"),
            make_pair("<=", "sle"),
            make_pair(">=", "sge"),

            make_pair("+", "addu"),
            make_pair("-", "subu"),
            make_pair("*", "mul"),
            make_pair("/", "div"),
            make_pair("%", "rem"),
    };

    void writeString(char *curr);

    void preCB1(AST::ASTNode *node);

    void preCB2(AST::ASTNode *node);

    void postCB2(AST::ASTNode *node);


    std::string generate_prints();

    std::string generate_printc();

    std::string generate_printi();

    std::string generate_printb();

    std::string generate_len();

    std::string generate_halt();

    std::string generate_getchar();


    explicit CodeGenerator(AST::Program *program);

    void preOrder(AST::ASTNode *node);

    void prePostOrder(AST::ASTNode *node);

    static void writeLine(const std::string &line) {
        std::cout << line << std::endl;
    }
};

#endif //CPSC_411_CODEGEN_H

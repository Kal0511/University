//
// Created by kal on 18/03/23.
//

#ifndef CPSC_411_SEMANTICCHECKER_H
#define CPSC_411_SEMANTICCHECKER_H

#include <cstring>
#include "AST.h"
#include "SymbolTable.h"

// used to check if the AST is valid according to the GOLF specifications
class SemanticChecker {
public:
    SymbolTable table;

    string currReturnType;

    bool isUnarySignPos = true;
    int forLevel = 0;
    bool returnFound = false;
    int count = 1;
    int varCount = 0;

    void preCB(AST::ASTNode *node);

    void postCB(AST::ASTNode *node);

    explicit SemanticChecker(AST::Program *program);

    void prePostOrder(AST::ASTNode *node);

};


#endif //CPSC_411_SEMANTICCHECKER_H

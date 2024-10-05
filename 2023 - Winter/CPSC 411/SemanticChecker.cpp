#include <cstring>
#include "AST.h"
#include "SymbolTable.h"
#include "SemanticChecker.h"


void SemanticChecker::preCB(AST::ASTNode *node) {
    if (dynamic_cast<AST::Block *>(node)) {
        table.pushScope();
    }
    if (auto decl = dynamic_cast<AST::Declaration *>(node)) {
        table.define(decl->variable->variableName, decl->variable->variableType, false, false, decl->lineno);
        decl->record = table.lookup(decl->variable->variableName);
        decl->record->num = 0;
        decl->num = count;
        count++;
        varCount++;
    }

    if (auto func = dynamic_cast<AST::Function *>(node)) {
        varCount = 0;
        table.pushScope();
        if (func->signature->parameters != nullptr) {
            for (const AST::Parameter *parameter: *func->signature->parameters) {
                Record *type = table.lookup(parameter->parameterType);
                if (!type->isType) {
                    cerr << "Error: Expected type" << endl;
                    exit(1);
                }
                table.define(parameter->parameterName, parameter->parameterType, false, false, parameter->lineno);
            }
        }
        currReturnType = func->signature->returnType;
        returnFound = false;
    }

    if (auto expr = dynamic_cast<AST::UnaryExpression *>(node)) {
        if (strcmp(expr->symbol, "-") == 0) {
            isUnarySignPos = false;
        }
    }

    if (auto lit = dynamic_cast<AST::Literal *>(node)) {
        long max = 2147483647;
        if (!isUnarySignPos) {
            max++;
        }
        try {
            if (strcmp(lit->type, "int") == 0 && stol(lit->data) > max) {
                cerr << "Error: Int out of range" << endl;
                exit(1);
            }
        } catch (std::out_of_range const &e) {
            cerr << "Error: Int out of range" << endl;
            exit(1);
        }
    }

    if (dynamic_cast<AST::For *>(node)) {
        forLevel++;
    }

    if (dynamic_cast<AST::Break *>(node)) {
        if (forLevel <= 0) {
            cerr << "Error: Not inside for loop" << endl;
            exit(1);
        }
    }
}

void SemanticChecker::postCB(AST::ASTNode *node) {
    if (dynamic_cast<AST::Block *>(node)) {
        table.popScope();
    }
    if (auto expr = dynamic_cast<AST::BinaryExpression *>(node)) {
        vector <vector<string>> row = table.typeMap.find(expr->symbol)->second;
        for (auto type: row) {
            if (expr->right->returnType == type[0] && expr->left->returnType == type[1]) {
                expr->returnType = type[2];
                break;
            }
        }
        if (expr->returnType.empty()) {
            cerr << "Error: Invalid types in expression at line no " << expr->lineno << endl;
            exit(1);
        }
    }
    if (auto expr = dynamic_cast<AST::UnaryExpression *>(node)) {
        if (strcmp(expr->symbol, "-") == 0) {
            isUnarySignPos = true;
        }
        if (expr->expression->returnType == "string") {
            cerr << "Error: Unary expression contains string" << endl;
            exit(1);
        }
        expr->returnType = expr->expression->returnType;
    }

    if (auto expr = dynamic_cast<AST::PrimaryExpression *>(node)) {
        if (auto primary = dynamic_cast<AST::OperandName *>(expr->operation)) {
            if (strcmp(primary->data, "halt") == 0) {
                returnFound = true;
            }

            Record *record = table.lookup(primary->data);
            primary->record = record;
            primary->num = record->num;

            if (record->isType) {
                cerr << "Error: operand can not be a type" << endl;
                exit(1);
            }

            expr->returnType = record->type;
            primary->returnType = record->type;
            for (auto arguments: expr->argumentsList) {
                if (arguments->size() != record->inputType.size()) {
                    cerr << "Error: Number of inputs invalid at " << expr->lineno << endl;
                    exit(1);
                }
                for (size_t i = 0; i < arguments->size(); i++) {
                    auto arg = (AST::Expression *) ((*arguments)[i]);
                    if (arg->returnType != record->inputType[i]) {
                        cerr << "Error: Type of input do not match" << endl;
                        exit(1);
                    }
                }
            }
        }
        if (auto primary = dynamic_cast<AST::Literal *>(expr->operation)) {
            if (!expr->argumentsList.empty()) {
                cerr << "Error: Cant call string" << endl;
                exit(1);
            }
            expr->returnType = primary->type;
        }
        if (auto primary = dynamic_cast<AST::Expression *>(expr->operation)) {
            if (!expr->argumentsList.empty()) {
                cerr << "Error: Cant call expression" << endl;
                exit(1);
            }
            expr->returnType = primary->returnType;
        }
    }

    if (auto assign = dynamic_cast<AST::Assignment *>(node)) {

        if (auto expr = dynamic_cast<AST::PrimaryExpression *>(assign->left)) {
            if (auto operation = dynamic_cast<AST::OperandName *>(expr->operation)) {
                Record *record = table.lookup(operation->data);
                if (record->type != assign->right->returnType) {
                    cerr << "Error: Cant assign" << endl;
                    exit(1);
                }
                if (record->isConst) {
                    cerr << "Error: Cant reassign const" << endl;
                    exit(1);
                }
                if (!expr->argumentsList.empty()) {
                    cerr << "Error: Cant assign to function" << endl;
                    exit(1);
                }
                return;
            }
        }
        cerr << "Error: Left side is not an identifier" << endl;
        exit(1);
    }

    if (auto returnStmt = dynamic_cast<AST::Return *>(node)) {
        if (returnStmt->condition == nullptr) {
            if (currReturnType != "void") {
                cerr << "Error: Return type should be void" << endl;
                exit(1);
            }
        } else {
            if (currReturnType != returnStmt->condition->returnType) {
                cerr << "Error: Return type does not match" << endl;
                exit(1);
            }
        }
        returnFound = true;
    }

    if (auto For = dynamic_cast<AST::For *>(node)) {
        forLevel--;
        if (For->condition->returnType != "bool") {
            cerr << "Error: For condition must result in bool" << endl;
            exit(1);
        }
    }

    if (auto func = dynamic_cast<AST::Function *>(node)) {
        if (strcmp(func->signature->returnType, "void") != 0 && !returnFound) {
            cerr << "Error: Return not found in " << func->functionName << endl;
            exit(1);
        }
        table.popScope();
        currReturnType = "";
        func->num = varCount;
    }

    if (auto If = dynamic_cast<AST::If *>(node)) {
        if (If->condition->returnType != "bool") {
            cerr << "Error: If condition must result in bool" << endl;
            exit(1);
        }
    }
}

SemanticChecker::SemanticChecker(AST::Program *program) {
    int mainFuncCount = 0;
    for (AST::ASTNode *child: program->children) {
        if (auto func = dynamic_cast<AST::Function *>(child)) {
            table.define(func->functionName, func->signature->getSignature(), false, false, func->lineno);
            if (func->signature->parameters != nullptr) {
                for (const AST::Parameter *parameter: *func->signature->parameters) {
                    Record *type = table.lookup(parameter->parameterType);
                    if (!type->isType) {
                        cerr << "Error: Expected type" << endl;
                        exit(1);
                    }
                    table.lookup(func->functionName)->inputType.emplace_back(parameter->parameterType);
                }
            }

            if (strcmp(func->functionName, "main") == 0) {
                if (strcmp(func->signature->returnType, "void") != 0) {
                    cerr << "Error: Main function return type is not void" << endl;
                    exit(1);
                }
                if (func->signature->parameters != nullptr) {
                    cerr << "Error: Main function can not have parameters" << endl;
                    exit(1);
                }
                mainFuncCount++;
                if (mainFuncCount > 1) {
                    cerr << "Error: Too many main function were found" << endl;
                    exit(1);
                }
            }
        }
        if (auto decl = dynamic_cast<AST::GlobalDeclaration *>(child)) {
            if (strcmp(decl->variable->variableName, "int") == 0 || strcmp(decl->variable->variableName, "bool") == 0 || strcmp(decl->variable->variableName, "string") == 0) {
                cerr << "Error: name cant be a type" << endl;
                exit(1);
            }
            table.define(decl->variable->variableName, decl->variable->variableType, false, false, decl->lineno, true);
            Record *type = table.lookup(decl->variable->variableType);
            if (!type->isType) {
                cerr << "Error: Expected type" << endl;
                exit(1);
            }
            decl->record = table.lookup(decl->variable->variableName);
            decl->record->num = count;
            decl->num = count;
            count++;
        }
    }
    if (mainFuncCount == 0) {
        cerr << "Error: Main function not found" << endl;
        exit(1);
    }

    prePostOrder(program);
}

void SemanticChecker::prePostOrder(AST::ASTNode *node) {
    preCB(node);
    for (AST::ASTNode *child: node->children) {
        prePostOrder(child);
    }
    postCB(node);
}


//
// Created by kal on 25/02/23.
//

#ifndef CPSC_411_AST_H
#define CPSC_411_AST_H

#include <utility>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace AST {

    class ASTNode {
    public:
        int lineno;
        vector<ASTNode *> children;
        string returnType;

        explicit ASTNode(int no) : lineno(no), children(), returnType() {}

        void preOrder(void (*pre)(ASTNode *)) {
            pre(this);
            for (ASTNode *child: children) {
                child->preOrder(pre);
            }
        }

        void postOrder(void (*post)(ASTNode *)) {
            for (ASTNode *child: children) {
                child->postOrder(post);
            }
            post(this);
        }

        void prePostOrder(void (*pre)(ASTNode *), void (*post)(ASTNode *)) {
            pre(this);
            for (ASTNode *child: children) {
                child->prePostOrder(pre, post);
            }
            post(this);
        }

        virtual void print(int level) {
            cout << string(level, '\t') + "NODE at " << lineno << endl;
        }
    };

    class Literal : public ASTNode {
    public:
        const char *data;
        const char *type;

        Literal(int no, const char *data, const char *type) : ASTNode(no), data(data), type(type) {}

        void print(int level) override {
            cout << string(level, '\t') << type << " [" << data << "] at line " << lineno << endl;
        }
    };

    class OperandName : public ASTNode {
    public:
        const char *data;

        OperandName(int no, const char *data) : ASTNode(no), data(data) {}

        void print(int level) override {
            cout << string(level, '\t') << "id [" << data << "] sig=" << returnType << " at line " << lineno << endl;
        }
    };

    class VarSpec : public ASTNode {
    public:
        const char *variableName;
        const char *variableType;

        VarSpec(int no, const char *name, const char *type) : ASTNode(no), variableName(name), variableType(type) {}

        void print(int level) override {
            cout << string(level, '\t') + "new id [" << variableName << "] at line " << lineno << endl;
            cout << string(level, '\t') + "id type [" << variableType << "] at line " << lineno << endl;
        }
    };

    class Declaration : public ASTNode {
    public:
        VarSpec *variable;

        Declaration(int no, VarSpec *variable) : ASTNode(no), variable(variable) {}

        void print(int level) override {
            cout << string(level, '\t') + "var at line " << lineno << endl;
            variable->print(level + 1);
        }
    };

    class GlobalDeclaration : public ASTNode {
    public:
        VarSpec *variable;

        GlobalDeclaration(int no, VarSpec *variable) : ASTNode(no), variable(variable) {}

        void print(int level) override {
            cout << string(level, '\t') + "global var at line " << lineno << endl;
            variable->print(level + 1);
        }
    };

    class Block : public ASTNode {
    public:
        vector<ASTNode *> *statements;

        explicit Block(int no, vector<ASTNode *> *data) : ASTNode(no), statements(data) {
            children = *statements;
        }

        void print(int level) override {
            cout << string(level, '\t') + "Block" << endl;
            for (ASTNode *node: *statements) {
                node->print(level + 1);
            }
        }
    };

    class Parameter : public ASTNode {
    public:
        const char *parameterName;
        const char *parameterType;

        Parameter(int no, const char *name, const char *type) : ASTNode(no), parameterName(name), parameterType(type) {}
    };

    class FunctionSignature : public ASTNode {
    public:
        vector<Parameter *> *parameters;
        const char *returnType;

        FunctionSignature(int no, vector<Parameter *> *parameters, const char *returnType) : ASTNode(no), parameters(parameters), returnType(returnType) {}

        string getSignature() const {
            string signature = "f(";
            if (parameters != nullptr) {
                for (Parameter *parameter: *parameters) {
                    signature += parameter->parameterType;
                }
            }
            signature += "):";
            signature += +returnType;
            return signature;
        }

        void print(int level) override {
            cout << string(level, '\t') + "sig" << endl;
            cout << string(level + 1, '\t') + "formals" << endl;
            if (parameters != nullptr) {
                for (Parameter *parameter: *parameters) {
                    cout << string(level + 2, '\t') + "formal" << endl;
                    cout << string(level + 3, '\t') + "new id [" + parameter->parameterName << "] at line " << parameter->lineno << endl;
                    cout << string(level + 3, '\t') + "id type [" + parameter->parameterType << "] at line " << parameter->lineno << endl;
                }
            }
            if (returnType == nullptr) {
                cout << string(level + 1, '\t') + "id type [$void]" << endl;
            } else {
                cout << string(level + 1, '\t') + "id type [" << string(returnType) << "] at line " << lineno << endl;
            }
        }
    };

    class Function : public ASTNode {
    public:
        const char *functionName;
        FunctionSignature *signature;
        Block *functionBody;

        Function(int no, const char *name, FunctionSignature *sign, Block *body) : ASTNode(no), functionName(name), signature(sign), functionBody(body) {
            children.push_back(functionBody);
        }

        void print(int level) override {
            cout << string(level, '\t') + "func at line " << lineno << endl;
            cout << string(level + 1, '\t') + "new id [" << functionName << "] at line " << lineno << endl;
            if (signature) {
                signature->print(level + 1);
            }
            if (functionBody) {
                functionBody->print(level + 1);
            }
        }
    };

    class Expression : public ASTNode {
    public:
        string returnType;

        explicit Expression(int no) : ASTNode(no), returnType() {}
    };

    class ExpressionStmt : public Expression {
    public:
        Expression *stmt;

        ExpressionStmt(int no, Expression *stmt) : Expression(no), stmt(stmt) {
            children.push_back(stmt);
        }

        void print(int level) override {
            cout << string(level, '\t') << "expr stmt at line " << lineno << endl;
            stmt->print(level + 1);
        }
    };

    class BinaryExpression : public Expression {
    public:
        const char *symbol;
        Expression *left;
        Expression *right;

        BinaryExpression(int no, const char *symbol, Expression *left, Expression *right) : Expression(no), symbol(symbol), left(left), right(right) {
            children.push_back(left);
            children.push_back(right);
        }

        void print(int level) override {
            cout << string(level, '\t') << string(symbol) << " at line " << lineno << endl;
            left->print(level + 1);
            right->print(level + 1);
        }
    };

    class UnaryExpression : public Expression {
    public:
        const char *symbol;
        Expression *expression;

        UnaryExpression(int no, const char *symbol, Expression *expression) : Expression(no), symbol(symbol), expression(expression) {
            children.push_back(expression);
        }

        void print(int level) override {
            cout << string(level, '\t') + "u" + string(symbol) + " at line " << lineno << endl;
            expression->print(level + 1);
        }
    };

    class PrimaryExpression : public Expression {
    public:
        ASTNode *operation;
        vector<vector<ASTNode *> *>
                argumentsList;

        PrimaryExpression(int no, ASTNode *operation) : Expression(no), operation(operation), argumentsList() {
            children.push_back(operation);
        }

        void update() {
            for (auto arguments: argumentsList) {
                if (arguments != nullptr) {
                    for (auto argument: *arguments) {
                        children.push_back((AST::Expression *) argument);
                    }
                }
            }
        }

        void print(int level) override {
            if (!argumentsList.empty()) {
                cout << string(level, '\t') + "func call" << endl;
                level++;
            }
            operation->print(level);
            if (!argumentsList.empty()) {
                cout << string(level, '\t') + "actuals" << endl;
            }
            for (vector<ASTNode *> *argumentList: argumentsList) {
                if (argumentList != nullptr) {
                    for (ASTNode *node: *argumentList) {
                        node->print(level + 1);
                    }
                }
            }
        }
    };

    class If : public ASTNode {
    public:
        Expression *condition;
        Block *trueBlock;
        Block *falseBlock;

        If(int no, Expression *condition, Block *t, Block *f) : ASTNode(no), condition(condition), trueBlock(t), falseBlock(f) {
            children.push_back(condition);
            children.push_back(trueBlock);
            if (falseBlock != nullptr) {
                children.push_back(falseBlock);
            }
        }

        void print(int level) override {
            cout << string(level, '\t') + "if at line " << lineno << endl;
            condition->print(level + 1);
            trueBlock->print(level + 1);
            if (falseBlock != nullptr) {
                falseBlock->print(level + 1);
            }
        }
    };

    class IfElse : public ASTNode {
    public:
        Expression *condition;
        Block *trueBlock;
        Block *falseBlock;

        IfElse(int no, Expression *condition, Block *t, Block *f) : ASTNode(no), condition(condition), trueBlock(t), falseBlock(f) {
            children.push_back(condition);
            children.push_back(trueBlock);
            if (falseBlock != nullptr) {
                children.push_back(falseBlock);
            }
        }

        void print(int level) override {
            cout << string(level, '\t') + "ifelse at line " << lineno << endl;
            condition->print(level + 1);
            trueBlock->print(level + 1);
            if (falseBlock != nullptr) {
                falseBlock->print(level + 1);
            }
        }
    };

    class For : public ASTNode {
    public:
        Expression *condition;
        Block *block;

        For(int no, Expression *condition, Block *block) : ASTNode(no), condition(condition), block(block) {
            if (this->condition == nullptr) {
                this->condition = new PrimaryExpression(no, new OperandName(no, "true"));
            }
            children.push_back(this->condition);
            children.push_back(block);
        }

        void print(int level) override {
            cout << string(level, '\t') + "for at line " << lineno << endl;
//            if (condition != nullptr) {
            condition->print(level + 1);
//            } else {
//                cout << string(level + 1, '\t') + "id [$true] at line " << lineno << endl;
//            }
            block->print(level + 1);
        }
    };

    class Return : public ASTNode {
    public:
        Expression *condition;

        Return(int no, Expression *condition) : ASTNode(no), condition(condition) {
            if (condition != nullptr) {
                children.push_back(condition);
            }
        }

        void print(int level) override {
            cout << string(level, '\t') + "return at line " << lineno << endl;
            if (condition != nullptr) {
                condition->print(level + 1);
            }
        }
    };

    class Break : public ASTNode {
    public:
        explicit Break(int no) : ASTNode(no) {}

        void print(int level) override {
            cout << string(level, '\t') + "break at line " << lineno << endl;
        }
    };

    class EmptyStmt : public ASTNode {
    public:
        explicit EmptyStmt(int no) : ASTNode(no) {}

        void print(int level) override {
            cout << string(level, '\t') + "empty stmt" << endl;
        }
    };

    class Assignment : public ASTNode {
    public:
        Expression *left;
        Expression *right;

        Assignment(int no, Expression *left, Expression *right) : ASTNode(no), left(left), right(right) {
            children.push_back(left);
            children.push_back(right);
        }

        void print(int level) override {
            cout << string(level, '\t') + "= at line " << lineno << endl;
            left->print(level + 1);
            right->print(level + 1);
        }
    };

    class Program : public ASTNode {
    public:

        Program() : ASTNode(0) {}

        void print(int level) override {
            cout << "Program" << endl;
            for (ASTNode *node: children) {
                node->print(level + 1);
            }
        }
    };
}

#endif //CPSC_411_AST_H
//
// Created by kal on 28/01/23.
//

#include <iostream>
#include <fstream>
#include "scanner.h"
#include "parser.hh"
#include "AST.h"
#include "SymbolTable.h"
#include "SemanticChecker.h"
#include "codeGen.h"

using namespace std;

using Token = GOLF::Parser::token::yytokentype;

void printToken(Token token, const string &id, int line) {
    if (token == Token::IDENTIFIER_T) {
        cout << "Identifier: " << id << " at line " << line << endl;
    } else if (token == Token::NUMBER_T) {
        cout << "Number: " << id << " at line " << line << endl;
    } else if (token == Token::STRING_T) {
        cout << "String: \"" << id << "\" at line " << line << endl;
    } else if (token >= Token::BREAK_T && token <= Token::VAR_T) {
        cout << "Keyword: " << id << " at line " << line << endl;
    } else if (token == Token::SEMI_COLON_T) {
        cout << "Operator: ';' at line " << line << endl;
    } else {
        cout << "Operator: '" << id << "' at line " << line << endl;
    }
}

int main(int argc, char **argv) {
    // Filename was not provided - throw error
    if (argc < 2) {
        cerr << "Error: File Argument Not Found" << endl;
        return 1;
    }

    // Too many arguments were provided - throw error
    if (argc > 2) {
        cerr << "Error: Too Many Arguments" << endl;
        return 1;
    }

    // Open filename provided and throw error if file cant be opened
    ifstream file(argv[1]);
    if (!file.good()) {
        cerr << "Error: Can't open file" << endl;
        exit(1);
    }

    GOLF::Lexer lexer(&file);

//    // Loop through all the tokens found and print them
//    Token token;
//    auto *yylval = new GOLF::Parser::semantic_type();
//    auto *loc = new GOLF::Parser::location_type();
//    while ((token = (Token) lexer.yylex(yylval, loc)) != Token::YYEOF) {
//        printToken(token, lexer.YYText(), lexer.lineno());
//    }

    AST::Program program;
    GOLF::Parser parser(lexer, program);
    parser.parse();

    SemanticChecker checker(&program);

    // Print the AST
//    program.print(0);

    CodeGenerator generator(&program);

    return 0;
}
//
// Created by kal on 28/01/23.
//

#include <iostream>
#include <fstream>
#include <FlexLexer.h>
#include "token.h"

using namespace std;

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

    yyFlexLexer lexer(&file);

    // Loop through all the tokens found and print them
    TokenType token;
    while ((token = (TokenType) lexer.yylex()) != 0) {
        printToken(token, string(lexer.YYText()), lexer.lineno());
    }

    return 0;
}
//
// Created by kal on 02/02/23.
//

#ifndef CPSC_411_TOKEN_H
#define CPSC_411_TOKEN_H

#pragma once

#include <iostream>

using namespace std;

// All tokens the scanner can identify
enum TokenType {
    EOF_T = 0,
    IDENTIFIER_T = 1,
    STRING_T,
    NUMBER_T,

    // KEYWORD
    BREAK_T,
    ELSE_T,
    FOR_T,
    FUNC_T,
    IF_T,
    RETURN_T,
    VAR_T,

    // OPERATOR
    ADD_T,
    SUB_T,
    MULT_T,
    DIV_T,
    MOD_T,

    AND_T,
    OR_T,

    EQUAL_T,
    NOT_EQUAL_T,

    LT_T,
    GT_T,
    LE_T,
    GE_T,

    NOT_T,
    ASSIGN_T,

    BRACKET_OPEN_T,
    BRACKET_CLOSE_T,

    CURLY_OPEN_T,
    CURLY_CLOSE_T,

    COMMA_T,
    SEMI_COLON_T
};

// Utility function to print tokens and associated information
inline void printToken(TokenType token, const string &id, int line) {
    if (token == IDENTIFIER_T) {
        cout << "Identifier: " << id << " at line " << line << endl;
    } else if (token == NUMBER_T) {
        cout << "Number: " << id << " at line " << line << endl;
    } else if (token == STRING_T) {
        cout << "String: \"" << id << "\" at line " << line << endl;
    } else if (token >= BREAK_T && token <= VAR_T) {
        cout << "Keyword: " << id << " at line " << line << endl;
    } else if (token == SEMI_COLON_T) {
        cout << "Operator: ';' at line " << line << endl;
    } else {
        cout << "Operator: '" << id << "' at line " << line << endl;
    }
}

// format escape character appropriately
inline char *formatEscapeChar(char *str) {
    switch (str[0]) {
        case '\a':
            return (char *) "\\a";
        case '\b':
            return (char *) "\\b";
        case '\t':
            return (char *) "\\t";
        case '\n':
            return (char *) "\\n";
        case '\v':
            return (char *) "\\v";
        case '\f':
            return (char *) "\\f";
        case '\r':
            return (char *) "\\r";
        case '\0':
            return (char *) "NULL";
        default:
            return str;
    }
}

#endif // CPSC_411_TOKEN_H

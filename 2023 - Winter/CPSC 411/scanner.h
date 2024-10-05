//
// Created by kal on 22/02/23.
//

#ifndef CPSC_411_SCANNER_H
#define CPSC_411_SCANNER_H

#undef yyFlexLexer

#include <FlexLexer.h>

#include "parser.hh"
#include "location.hh"

namespace GOLF {
    class Lexer : public yyFlexLexer {
    public:
        explicit Lexer(std::istream *in) : yyFlexLexer(in) {}

        ~Lexer() override = default;

        virtual int yylex(GOLF::Parser::semantic_type *yylval, GOLF::Parser::location_type *loc);
    };
}

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

#endif //CPSC_411_SCANNER_H

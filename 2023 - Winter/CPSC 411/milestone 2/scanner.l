%option c++
%option yyclass="GOLF::Lexer"
%option yylineno
%option noyywrap

/* Declarations */

%{
    #include <iostream>
    #include <fstream>
    #include "scanner.h"
    #include "parser.hh"

    using Token = GOLF::Parser::token::yytokentype;

    #undef  YY_DECL
    #define YY_DECL int GOLF::Lexer::yylex(GOLF::Parser::semantic_type *yylval, GOLF::Parser::location_type *loc)

    #define YY_USER_ACTION loc->step(); loc->columns(yyleng);

    Token lastTokenType = Token::YYEOF;

    int numOfWarnings = 0;
    void incrementWarnings(){
        numOfWarnings++;
        if(numOfWarnings >=10){
            fprintf(stderr, "Error: too many warnings");
            exit(1);
        }
    }

    int curlyBracketLevel = 0;
%}

/* Definitions */

alpha               [a-zA-Z]
letter              {alpha}|_
digit               [0-9]

num                 {digit}+
ID                  {letter}({letter}|{digit})*

badEscapeChar       \\[^bfnrt\\"]
escapeChar          \\[bfnrt\\"]

invalidAnd          &[^&]+&
invalidOr           [|][^|]+[|]

nonAscii            [^\x00-\x7F]

/* States */

%x STRING

%% /* Rules */

[ \t\r]+            ;
\n                  {
                        if(lastTokenType==Token::IDENTIFIER_T || lastTokenType==Token::STRING_T || lastTokenType==Token::NUMBER_T || lastTokenType==Token::RETURN_T || lastTokenType==Token::BREAK_T || lastTokenType==Token::BRACKET_CLOSE_T || lastTokenType==Token::CURLY_CLOSE_T){
                            lastTokenType=Token::SEMI_COLON_T;
                            yyless(0);
                            return lastTokenType;
                        }
                        loc->lines();
                        loc->step();
                    }
<INITIAL><<EOF>>    {
                        if(lastTokenType==Token::IDENTIFIER_T || lastTokenType==Token::STRING_T || lastTokenType==Token::NUMBER_T || lastTokenType==Token::RETURN_T || lastTokenType==Token::BREAK_T || lastTokenType==Token::BRACKET_CLOSE_T || lastTokenType==Token::CURLY_CLOSE_T){
                            lastTokenType=Token::SEMI_COLON_T;
                            return lastTokenType;
                        }
                        return 0;
                    }

{invalidAnd}        { fprintf(stderr, "Error: bitwise AND not supported at line %d\n", yylineno); exit(1);}
{invalidOr}         { fprintf(stderr, "Error: bitwise OR not supported at line %d\n", yylineno); exit(1);}

break           { lastTokenType=Token::BREAK_T; return lastTokenType;}
return          { lastTokenType=Token::RETURN_T; return lastTokenType;}
else            { lastTokenType=Token::ELSE_T; return lastTokenType;}
for             { lastTokenType=Token::FOR_T; return lastTokenType;}
func            { lastTokenType=Token::FUNC_T; return lastTokenType;}
if              { lastTokenType=Token::IF_T; return lastTokenType;}
var             { lastTokenType=Token::VAR_T; return lastTokenType;}

"+"             { lastTokenType=Token::ADD_T; return lastTokenType;}
"-"             { lastTokenType=Token::SUB_T; return lastTokenType;}
"*"             { lastTokenType=Token::MULT_T; return lastTokenType;}
"/"             { lastTokenType=Token::DIV_T; return lastTokenType;}
"%"             { lastTokenType=Token::MOD_T; return lastTokenType;}
"&&"            { lastTokenType=Token::AND_T; return lastTokenType;}
"||"            { lastTokenType=Token::OR_T; return lastTokenType;}
"=="            { lastTokenType=Token::EQUAL_T; return lastTokenType;}
"!="            { lastTokenType=Token::NOT_EQUAL_T; return lastTokenType;}
"<"             { lastTokenType=Token::LT_T; return lastTokenType;}
">"             { lastTokenType=Token::GT_T; return lastTokenType;}
"<="            { lastTokenType=Token::LE_T; return lastTokenType;}
">="            { lastTokenType=Token::GE_T; return lastTokenType;}
"!"             { lastTokenType=Token::NOT_T; return lastTokenType;}
"="             { lastTokenType=Token::ASSIGN_T; return lastTokenType;}
"("             { lastTokenType=Token::BRACKET_OPEN_T; return lastTokenType;}
")"             { lastTokenType=Token::BRACKET_CLOSE_T; return lastTokenType;}
"{"             { lastTokenType=Token::CURLY_OPEN_T; curlyBracketLevel++; return lastTokenType;}
"}"             {
                    if(lastTokenType!=Token::SEMI_COLON_T && curlyBracketLevel != 0){
                        lastTokenType=Token::SEMI_COLON_T;
                        yyless(0);
                        return lastTokenType;
                    }
                    if(curlyBracketLevel != 0){
                        curlyBracketLevel--;
                    }
                    lastTokenType=Token::CURLY_CLOSE_T;
                    return lastTokenType;
                }
","             { lastTokenType=Token::COMMA_T; return lastTokenType;}
";"             { lastTokenType=Token::SEMI_COLON_T; return lastTokenType;}

{ID}            { yylval->sval=strdup(yytext); lastTokenType=Token::IDENTIFIER_T; return lastTokenType;}
{num}           { yylval->sval=strdup(yytext); lastTokenType=Token::NUMBER_T; return lastTokenType;}

\/\/.*

\"              { BEGIN(STRING);}
<STRING>{
    \"                  { BEGIN(INITIAL); yytext[yyleng-1]='\0'; yylval->sval=strdup(yytext); lastTokenType=Token::STRING_T; return lastTokenType;}
    <<EOF>>             { fprintf(stderr, "Error: EOF at line %d\n", yylineno); exit(1);}
    \n                  { fprintf(stderr, "Error: \\n at line %d\n", yylineno-1); exit(1);}
    {badEscapeChar}     { fprintf(stderr, "Error: invalid escape char '%s' at line %d\n", formatEscapeChar(yytext+yyleng-1), yylineno); exit(1);}
    {escapeChar}        { yymore();}
    .                   { yymore();}
}

{nonAscii}      { fprintf(stderr, "Warning: non ascii character at line %d\n", yylineno); incrementWarnings();}
.               { fprintf(stderr, "Warning: illegal character '%s' at line %d\n", formatEscapeChar(yytext), yylineno); incrementWarnings();}

%% 

/* User routines */

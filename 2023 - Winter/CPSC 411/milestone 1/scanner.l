%option c++
%option yylineno
%option noyywrap

/* Declarations */

%{
    #include <iostream>
    #include <fstream>
    #include "token.h"

    TokenType lastTokenType = EOF_T;

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
                        if(lastTokenType==IDENTIFIER_T || lastTokenType==STRING_T || lastTokenType==NUMBER_T || lastTokenType==RETURN_T || lastTokenType==BREAK_T || lastTokenType==BRACKET_CLOSE_T || lastTokenType==CURLY_CLOSE_T){
                            lastTokenType=SEMI_COLON_T;
                            yyless(0);
                            return lastTokenType;
                        }
                    }
<INITIAL><<EOF>>    {
                        if(lastTokenType==IDENTIFIER_T || lastTokenType==STRING_T || lastTokenType==NUMBER_T || lastTokenType==RETURN_T || lastTokenType==BREAK_T || lastTokenType==BRACKET_CLOSE_T || lastTokenType==CURLY_CLOSE_T){
                            lastTokenType=SEMI_COLON_T;
                            return lastTokenType;
                        }
                        return 0;
                    }

{invalidAnd}        { fprintf(stderr, "Error: bitwise AND not supported at line %d\n", yylineno); exit(1);}
{invalidOr}         { fprintf(stderr, "Error: bitwise OR not supported at line %d\n", yylineno); exit(1);}

break           { lastTokenType=BREAK_T; return lastTokenType;}
return          { lastTokenType=RETURN_T; return lastTokenType;}
else            { lastTokenType=ELSE_T; return lastTokenType;}
for             { lastTokenType=FOR_T; return lastTokenType;}
func            { lastTokenType=FUNC_T; return lastTokenType;}
if              { lastTokenType=IF_T; return lastTokenType;}
var             { lastTokenType=VAR_T; return lastTokenType;}

"+"             { lastTokenType=ADD_T; return lastTokenType;}
"-"             { lastTokenType=SUB_T; return lastTokenType;}
"*"             { lastTokenType=MULT_T; return lastTokenType;}
"/"             { lastTokenType=DIV_T; return lastTokenType;}
"%"             { lastTokenType=MOD_T; return lastTokenType;}
"&&"            { lastTokenType=AND_T; return lastTokenType;}
"||"            { lastTokenType=OR_T; return lastTokenType;}
"=="            { lastTokenType=EQUAL_T; return lastTokenType;}
"!="            { lastTokenType=NOT_EQUAL_T; return lastTokenType;}
"<"             { lastTokenType=LT_T; return lastTokenType;}
">"             { lastTokenType=GT_T; return lastTokenType;}
"<="            { lastTokenType=LE_T; return lastTokenType;}
">="            { lastTokenType=GE_T; return lastTokenType;}
"!"             { lastTokenType=NOT_T; return lastTokenType;}
"="             { lastTokenType=ASSIGN_T; return lastTokenType;}
"("             { lastTokenType=BRACKET_OPEN_T; return lastTokenType;}
")"             { lastTokenType=BRACKET_CLOSE_T; return lastTokenType;}
"{"             { lastTokenType=CURLY_OPEN_T; curlyBracketLevel++; return lastTokenType;}
"}"             {
                    if(lastTokenType!=SEMI_COLON_T && curlyBracketLevel != 0){
                        lastTokenType=SEMI_COLON_T;
                        yyless(0);
                        return lastTokenType;
                    }
                    if(curlyBracketLevel != 0){
                        curlyBracketLevel--;
                    }
                    lastTokenType=CURLY_CLOSE_T;
                    return lastTokenType;
                }
","             { lastTokenType=COMMA_T; return lastTokenType;}
";"             { lastTokenType=SEMI_COLON_T; return lastTokenType;}

{ID}            { lastTokenType=IDENTIFIER_T; return lastTokenType;}
{num}           { lastTokenType=NUMBER_T; return lastTokenType;}

\/\/.*

\"              { BEGIN(STRING);}
<STRING>{
    \"                  { BEGIN(INITIAL); yytext[yyleng-1]='\0'; lastTokenType=STRING_T; return lastTokenType;}
    <<EOF>>             { fprintf(stderr, "Error: EOF at line %d\n", yylineno); exit(1);}
    \n                { fprintf(stderr, "Error: \\n at line %d\n", yylineno-1); exit(1);}
    {badEscapeChar}     { fprintf(stderr, "Error: invalid escape char '%s' at line %d\n", formatEscapeChar(yytext+yyleng-1), yylineno); exit(1);}
    {escapeChar}        { yymore();}
    .                   { yymore();}
}

{nonAscii}      { fprintf(stderr, "Warning: non ascii character at line %d\n", yylineno); incrementWarnings();}
.               { fprintf(stderr, "Warning: illegal character '%s' at line %d\n", formatEscapeChar(yytext), yylineno); incrementWarnings();}

%% 

/* User routines */

%skeleton "lalr1.cc"
%language "c++"
%require "3.7"

%defines
%define api.namespace {GOLF}
%define api.parser.class {Parser}
%define parse.trace

%locations
%define parse.error custom

%code requires{
    namespace GOLF{
        class Lexer;
    }
    #include "AST.h"
}

%parse-param { GOLF::Lexer& lexer }
%parse-param { AST::Program& program }

%code{
    #include <iostream>
    #include "scanner.h"

    #undef yylex
    #define yylex lexer.yylex
}

%union{
    int ival;
    const char* sval;
    AST::ASTNode* node;
    std::vector<AST::ASTNode*>*list;
    AST::FunctionSignature* signature;
}

%token <sval> IDENTIFIER_T "identifier"
%token <sval> STRING_T "string"
%token <sval> NUMBER_T "number"

%token BREAK_T "BREAK"
%token ELSE_T "ELSE"
%token FOR_T "FOR"
%token FUNC_T "FUNC"
%token IF_T "IF"
%token RETURN_T "RETURN"
%token VAR_T "VAR"

%token ADD_T "+"
%token SUB_T "-"
%token MULT_T "*"
%token DIV_T "/"
%token MOD_T "%"
%token AND_T "&&"
%token OR_T "||"
%token EQUAL_T "=="
%token NOT_EQUAL_T "!="
%token LT_T "<"
%token GT_T ">"
%token LE_T "<="
%token GE_T ">="
%token NOT_T "!"
%token ASSIGN_T "="
%token BRACKET_OPEN_T "("
%token BRACKET_CLOSE_T ")"
%token CURLY_OPEN_T "{"
%token CURLY_CLOSE_T "}"
%token COMMA_T ","
%token SEMI_COLON_T ";"

%left OR_T
%left AND_T
%left EQUAL_T NOT_EQUAL_T LT_T GT_T LE_T GE_T
%left ADD_T SUB_T
%left MULT_T DIV_T MOD_T

%start Source

%type <node> TopLevelDecl Declaration FunctionDecl VarDecl VarSpec ParameterDecl Expression BreakStmt ReturnStmt SimpleStmt Statement FunctionBody Block IfStmt ForStmt ExpressionStmt Assignment UnaryExpr PrimaryExpr Operand Literal OperandName Condition

%type <signature> Signature

%type <list> ParameterList Parameters StatementList ExpressionList Arguments

%type <sval> FunctionName Result Type TypeName UnaryOperator

%%

UnaryOperator   : SUB_T                                                             {$$ = "-";}
                | NOT_T                                                             {$$ = "!";}
                ;

Source          : %empty
                | Source TopLevelDecl SEMI_COLON_T                                  {program.topLevel.push_back($2);}
                ;

TopLevelDecl    : Declaration                                                       {$$ = new AST::GlobalDeclaration(@$.begin.line,(AST::VarSpec*)$1);}
                | FunctionDecl                                                      {$$ = $1;}
                ;

Declaration     : VarDecl                                                           {$$ = $1;}
                ;

VarDecl         : VAR_T VarSpec                                                     {$$ = $2;}
                ;
VarSpec         : IDENTIFIER_T Type                                                 {$$ = new AST::VarSpec(@$.begin.line, $1, $2);}
                ;

Type            : TypeName                                                          {$$ = $1;}
                ;
TypeName        : IDENTIFIER_T                                                      {$$ = $1;}
                ;

FunctionDecl    : FUNC_T FunctionName Signature FunctionBody                        {$$ = new AST::Function(@$.begin.line, $2, $3, (AST::Block *)$4);}
                ;

FunctionName    : IDENTIFIER_T                                                      {$$ = $1;}
                ;

Signature       : Parameters                                                        {$$ = new AST::FunctionSignature(@$.begin.line ,(std::vector<AST::Parameter *>*)$1,nullptr);}
                | Parameters Result                                                 {$$ = new AST::FunctionSignature(@$.begin.line,(std::vector<AST::Parameter *>*)$1, $2);}
                ;

Parameters      : BRACKET_OPEN_T BRACKET_CLOSE_T                                    {$$ = nullptr;}
                | BRACKET_OPEN_T ParameterList BRACKET_CLOSE_T                      {$$ = $2;}
                | BRACKET_OPEN_T ParameterList COMMA_T BRACKET_CLOSE_T              {$$ = $2;}
                ;
ParameterList   : ParameterDecl                                                     {$$ = new std::vector<AST::ASTNode *>(); $$->push_back($1);}
                | ParameterList COMMA_T ParameterDecl                               {$$->push_back($3);}
                ;
ParameterDecl   : IDENTIFIER_T Type                                                 {$$ = new AST::Parameter(@$.begin.line,$1, $2);}
                ;

Result          : Type                                                              {$$ = $1;}
                ;

Arguments       : BRACKET_OPEN_T BRACKET_CLOSE_T                                        {$$ = nullptr;}
                | BRACKET_OPEN_T ExpressionList BRACKET_CLOSE_T                         {$$ = $2;}
                | BRACKET_OPEN_T ExpressionList COMMA_T BRACKET_CLOSE_T                 {$$ = $2;}
                ;

ExpressionList  : Expression                                            {$$ = new std::vector<AST::ASTNode *>(); $$->push_back($1);}
                | ExpressionList COMMA_T Expression                     {$$->push_back($3);}
                ;

FunctionBody    : Block                                                 {$$ = $1;}
                ;

Block           : CURLY_OPEN_T StatementList CURLY_CLOSE_T              {$$ = new AST::Block(@$.begin.line,$2);}
                ;
StatementList   : %empty                                                {$$ = new std::vector<AST::ASTNode *>();}
                | StatementList Statement ";"                           {$$->push_back($2);}
                ;

Statement       : Declaration                                           {$$ = new AST::Declaration(@$.begin.line,(AST::VarSpec*)$1);}
                | SimpleStmt                                            {$$ = $1;}
                | ReturnStmt                                            {$$ = $1;}
                | BreakStmt                                             {$$ = $1;}
                | Block                                                 {$$ = $1;}
                | IfStmt                                                {$$ = $1;}
                | ForStmt                                               {$$ = $1;}
                ;

SimpleStmt      : %empty                                                {$$ = new AST::EmptyStmt(@$.begin.line);}
                | ExpressionStmt                                        {$$ = new AST::ExpressionStmt(@$.begin.line,(AST::Expression*)$1);}
                | Assignment                                            {$$ = $1;}
                ;

ExpressionStmt  : Expression                                            {$$ = $1;}
                ;

Expression      : UnaryExpr                                             {$$ = $1;}
                | Expression EQUAL_T     Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"==",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression NOT_EQUAL_T Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"!=",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression AND_T       Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"&&",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression OR_T        Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"||",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression ADD_T       Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"+",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression SUB_T       Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"-",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression MULT_T      Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"*",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression DIV_T       Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"/",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression MOD_T       Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"%",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression LT_T        Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"<",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression GT_T        Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,">",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression LE_T        Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,"<=",(AST::Expression*)$1,(AST::Expression*)$3);}
                | Expression GE_T        Expression                     {$$ = new AST::BinaryExpression(@$.begin.line,">=",(AST::Expression*)$1,(AST::Expression*)$3);}
                ;

UnaryExpr       : PrimaryExpr                                           {$$ = $1;}
                | UnaryOperator UnaryExpr                               {$$ = new AST::UnaryExpression(@$.begin.line,$1,(AST::Expression*)$2);}
                ;

PrimaryExpr     : Operand                                               {$$ = new AST::PrimaryExpression(@$.begin.line,$1);}
                | PrimaryExpr Arguments                                 {((AST::PrimaryExpression*)$$)->argumentsList.push_back($2);}
                ;

Operand         : Literal                                               {$$ = $1;}
                | OperandName                                           {$$ = $1;}
                | BRACKET_OPEN_T Expression BRACKET_CLOSE_T             {$$ = $2;}
                ;

Literal         : NUMBER_T                                              {$$ = new AST::Literal(@$.begin.line, $1, "number");}
                | STRING_T                                              {$$ = new AST::Literal(@$.begin.line, $1, "string");}
                ;

OperandName     : IDENTIFIER_T                                          {$$ = new AST::OperandName(@$.begin.line, $1);}
                ;

Assignment      : Expression ASSIGN_T Expression                        {$$ = new AST::Assignment(@$.begin.line, $1, $3);}
                ;

IfStmt          : IF_T Expression Block                                 {$$ = new AST::If(@$.begin.line,(AST::Expression*)$2,$3,nullptr);}
                | IF_T Expression Block ELSE_T Block                    {$$ = new AST::IfElse(@$.begin.line,(AST::Expression*)$2,$3,$5);}
                | IF_T Expression Block ELSE_T IfStmt                   {$$ = new AST::IfElse(@$.begin.line,(AST::Expression*)$2,$3,$5);}
                ;

ForStmt         : FOR_T Block                                           {$$ = new AST::For(@$.begin.line,nullptr,(AST::Block*)$2);}
                | FOR_T Condition Block                                 {$$ = new AST::For(@$.begin.line,(AST::Expression*)$2,(AST::Block*)$3);}
                ;

Condition       : Expression                                            {$$ = $1;}
                ;

ReturnStmt      : RETURN_T                                              {$$ = new AST::Return(@$.begin.line, nullptr);}
                | RETURN_T Expression                                   {$$ = new AST::Return(@$.begin.line, (AST::Expression *)$2);}
                ;

BreakStmt       : BREAK_T                                               {$$ = new AST::Break(@$.begin.line);}
                ;

%%

void GOLF::Parser::error(const location_type &loc, const std::string &errmsg){
    std::cerr << "Error: " << errmsg << " at " << loc << std::endl;
}

void GOLF::Parser::report_syntax_error (const GOLF::Parser::context &ctx) const{
    const char *unexpectedToken = ctx.token() == symbol_kind::S_YYEMPTY ? "" : symbol_name(ctx.token());
    std::cerr<< "Error: Unexpected token {" << unexpectedToken<< "} at line "<< ctx.location().begin.line  << std::endl;
}
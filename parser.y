%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"

extern int yylineno;
extern FILE *yyin;

int yyget_lineno();
int yylex();
static void yyerror(const char *msg)
{
    fprintf(stderr, "%d: %s\n", yyget_lineno(), msg);
}
%}

%union
{
    char*       stringVal;
    long long   intVal;
    double      floatVal;
    pNode       node;      
}

%type<stringVal> STR ID;
%type<intVal> INT;
%type<floatVal> FLT;
%type<node> unary_exp post_unary call;
%type<node> factor mul_exp plus_exp shift_exp rel_exp eq_exp;
%type<node> b_and_exp b_xor_exp b_or_exp and_exp simple_exp cond_exp;
%type<node> exps assignment lval;


%defines
%locations
%pure-parser
%error-verbose

%right ASN ADD_ASN SUB_ASN MUL_ASN DIV_ASN MOD_ASN
%left ADD SUB MUL DIV MOD
%left B_AND B_XOR B_OR B_R B_L B_NOT B_AND_ASN B_XOR_ASN B_OR_ASN B_R_ASN B_L_ASN
%left LEQ GEQ LE GT EQ NEQ
%left AND OR NOT
%left DOT CLN QUE

%token SEM ID VAR FUN ASN RTN
%token IF ELS SWH CAS DFT
%token FOR WHL DO BRK CTN GTO
%token FLS TRU
%token INT FLT STR NUL
%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%token ADD SUB MUL DIV MOD ADD_ASN SUB_ASN MUL_ASN DIV_ASN MOD_ASN INC DEC
%token B_AND B_XOR B_OR B_R B_L B_NOT B_AND_ASN B_XOR_ASN B_OR_ASN B_R_ASN B_L_ASN
%token LEQ GEQ LE GT EQ NEQ
%token AND OR NOT
%token DOT CLN QUE CMA

%nonassoc EMPTY_ELS_CLAUSE
%nonassoc ELS
%nonassoc UNARY
%nonassoc EXP
%nonassoc CMA

%%

 /* ---- PROGRAM ---- */

program         : declarations              {}
                ;

declarations    : var_dec SEM declarations
                | fun_dec declarations
                | var_dec SEM
                | fun_dec
                | error SEM                 {}
                | error RBRACE              {}
                ;

 /* ---- VARIABLE DECLARATION ---- */

var_dec         : VAR vars
                ;

vars            : var_init CMA vars
                | var_init
                ;

var_init        : ID ASN exp
                | ID
                ;

 /* ---- FUNCTION DECLARATION ---- */

fun_dec         : FUN ID                    {}
                  LPAREN paras RPAREN       {}
                  block                     {}
                ;

paras           : para_list                 {}
                | /* empty */               {}
                ;

para_list       : para CMA para_list        {}
                | para                      {}
                ;

para            : ID                        {}

 /* ---- STATEMENTS ---- */

block           : LBRACE
                  stmts                     {}
                  RBRACE                    {}
                ;

stmts           : stmt stmts                {}
                | /* empty */
                ;

stmt            : declaration SEM           {}
                | if_stmt                   {}
                | switch_stmt               {}
                | for_stmt                  {}
                | while_stmt                {}
                | do_stmt                   {}
                | block                     {}
 /* label  */   | ID CLN stmt               {}
 /* return */   | RTN exp SEM               {}
                | BRK SEM                   {}
                | CTN SEM                   {}
                | GTO ID SEM                {}
                | exps SEM                  {}
                | SEM
  /* error */   | error SEM                 {}
                ;

declaration     : var_dec
                | fun_dec
                ;

if_stmt         : IF LPAREN exps RPAREN     {}
                  stmt                      {}
                  else_clause
                ;

else_clause     : ELS stmt                  {}
                | %prec EMPTY_ELS_CLAUSE
                ;

switch_stmt     : SWH LPAREN exps RPAREN    {}
                  LBRACE
                  case_stmts                {}
                  RBRACE
                ;

case_stmts      : case_stmt case_stmts      {}
                | /* empty */
                ;

case_stmt       : CAS exp CLN stmts         {}
                | DFT CLN stmts             {}
                ;

for_stmt        : FOR LPAREN
                  exps SEM                  {}
                  exps SEM                  {}
                  exps RPAREN               {}
                  stmt                      {}
                ;

while_stmt      : WHL LPAREN
                  exps RPAREN               {}
                  stmt
                ;

do_stmt         : DO stmt                   {}
                  WHL LPAREN exps RPAREN    {}
                ;

 /* ---- EXPRESSIONS ---- */

 /* 17 */
exps            : exp CMA exps              {}
                | exp %prec EXP {}
                ;

exp             : assignment
                ;
 /* 16 */
assignment      : lval ASN cond_exp                         { $$ = A_AssignExp(yylineno, $1, $3); }
                | lval ADD_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, ADD_OP, $1, $3)); }
                | lval SUB_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, SUB_OP, $1, $3)); }
                | lval MUL_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, MUL_OP, $1, $3)); }
                | lval DIV_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, DIV_OP, $1, $3)); }
                | lval MOD_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, MOD_OP, $1, $3)); }
                | lval B_AND_ASN cond_exp %prec ASN         { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, B_AND_OP, $1, $3)); }
                | lval B_XOR_ASN cond_exp %prec ASN         { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, B_XOR_OP, $1, $3)); }
                | lval B_OR_ASN cond_exp %prec ASN          { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, B_OR_OP, $1, $3)); }
                | lval B_R_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, B_R_OP, $1, $3)); }
                | lval B_L_ASN cond_exp %prec ASN           { $$ = A_AssignExp(yylineno, $1, A_BinaryExp(yylineno, B_L_OP, $1, $3)); }
                | cond_exp                                  { $$ = $1; }
                ;

 /* Left-value */
lval            : lval LBRACKET exp RBRACKET    {}
                | lval DOT ID                   {}
                | ID                            { $$ = A_IdExp(yylineno, $1); }
                ;

 /* ? : */
cond_exp        : simple_exp QUE exps CLN exps  { $$ = A_TrinaryExp(yylineno, $1, $3, $5); }
                | simple_exp                    { $$ = $1; }
                ;

 /* 15 */
simple_exp      : simple_exp OR and_exp     { $$ = A_BinaryExp(yylineno, OR_OP, $1, $3); }
                | and_exp                   { $$ = $1; }
                ;

 /* 14 */
and_exp         : and_exp AND b_or_exp      { $$ = A_BinaryExp(yylineno, AND_OP, $1, $3); }
                | b_or_exp                  { $$ = $1; }
                ;

 /* 13 */
b_or_exp        : b_or_exp B_OR b_xor_exp   { $$ = A_BinaryExp(yylineno, B_OR_OP, $1, $3); }
                | b_xor_exp                 { $$ = $1; }
                ;

 /* 12 */
b_xor_exp       : b_xor_exp B_XOR b_and_exp { $$ = A_BinaryExp(yylineno, B_XOR_OP, $1, $3); }
                | b_and_exp                 { $$ = $1; }
                ;

 /* 11 */
b_and_exp       : b_and_exp B_AND eq_exp    { $$ = A_BinaryExp(yylineno, B_AND_OP, $1, $3); }
                | eq_exp                    { $$ = $1; }
                ;

 /* 10 */
eq_exp          : eq_exp EQ rel_exp                 { $$ = A_BinaryExp(yylineno, EQ_OP, $1, $3); }
                | eq_exp NEQ rel_exp %prec EQ       { $$ = A_BinaryExp(yylineno, NEQ_OP, $1, $3); }
                | rel_exp                           { $$ = $1; }
                ;

 /* 9 */
rel_exp         : rel_exp LEQ shift_exp             { $$ = A_BinaryExp(yylineno, LEQ_OP, $1, $3); }
                | rel_exp GEQ shift_exp %prec LEQ   { $$ = A_BinaryExp(yylineno, GEQ_OP, $1, $3); }
                | rel_exp LE shift_exp %prec LEQ    { $$ = A_BinaryExp(yylineno, LE_OP, $1, $3); }
                | rel_exp GT shift_exp %prec LEQ    { $$ = A_BinaryExp(yylineno, GT_OP, $1, $3); }
                | shift_exp                         { $$ = $1; }
                ;

 /* 7 << >> */
shift_exp       : shift_exp B_L plus_exp            { $$ = A_BinaryExp(yylineno, B_L_OP, $1, $3); }
                | shift_exp B_R plus_exp %prec B_L  { $$ = A_BinaryExp(yylineno, B_R_OP, $1, $3); }
                | plus_exp                          { $$ = $1; }
                ;

 /* 6 */
plus_exp        : plus_exp ADD mul_exp              { $$ = A_BinaryExp(yylineno, ADD_OP, $1, $3); }
                | plus_exp SUB mul_exp %prec SUB    { $$ = A_BinaryExp(yylineno, SUB_OP, $1, $3); }
                | mul_exp                           { $$ = $1; }
                ;

 /* 5 */
mul_exp         : mul_exp MUL unary_exp             { $$ = A_BinaryExp(yylineno, MUL_OP, $1, $3); }
                | mul_exp DIV unary_exp %prec MUL   { $$ = A_BinaryExp(yylineno, DIV_OP, $1, $3); }
                | mul_exp MOD unary_exp %prec MUL   { $$ = A_BinaryExp(yylineno, MOD_OP, $1, $3); }
                | unary_exp                         { $$ = $1; }
                ;

 /* 3 */
unary_exp       : ADD post_unary %prec UNARY    { $$ = A_PreUnaryExp(yylineno, ADD_OP, $2); }
                | SUB post_unary %prec UNARY    { $$ = A_PreUnaryExp(yylineno, SUB_OP, $2); }
                | INC post_unary %prec UNARY    { $$ = A_PreUnaryExp(yylineno, INC_OP, $2); }
                | DEC post_unary %prec UNARY    { $$ = A_PreUnaryExp(yylineno, DEC_OP, $2); }
                | NOT post_unary %prec UNARY    { $$ = A_PreUnaryExp(yylineno, NOT_OP, $2); }
                | B_NOT post_unary %prec UNARY  { $$ = A_PreUnaryExp(yylineno, B_NOT_OP, $2); }
                | post_unary %prec UNARY        { $$ = $1; }
                ;

 /* 2 */
post_unary      : factor INC                { $$ = A_PostUnaryExp(yylineno, INC_OP, $1); }
                | factor DEC %prec INC      { $$ = A_PostUnaryExp(yylineno, DEC_OP, $1); }
                | call                      { $$ = $1; }
                | lval                      { $$ = $1; }
                | factor                    { $$ = $1; }
                ;

call            : ID LPAREN args RPAREN     {  }
                ;

args            : arg_list                  {  }
                | /* empty */               {  }
                ;

arg_list        : arg CMA arg_list          {  }
                | arg                       {  }
                ;

arg             : exp                       {  }
                ;

factor          : INT                       { $$ = A_IntExp(yylineno, $1); }
                | FLT                       { $$ = A_FloatExp(yylineno, $1); }
                | STR                       { $$ = A_StringExp(yylineno, $1); }
                | TRU                       { $$ = A_BooleanExp(yylineno, 1); }
                | FLS                       { $$ = A_BooleanExp(yylineno, 0); }
                | NUL                       { $$ = A_NullExp(yylineno); }
 /*               | object */
                ;


%%

int main(int argc, char **argv)
{
    if(argc > 1)
        yyin = fopen(argv[1], "r");
    else
        yyin = stdin;
    yyparse();
    return 0;
}
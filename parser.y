%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "absyn.h"

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
}

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
%token INT FLT STR
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

program         : declarations
                ;

declarations    : var_dec SEM declarations
                | fun_dec declarations
                | var_dec SEM
                | fun_dec
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

stmts           : stmt SEM stmts            {}
                | /* empty */
                ;

stmt            : declaration               {}
                | if_stmt                   {}
                | switch_stmt               {}
                | CAS CLN stmt              {}
                | DFT CLN stmt              {}
 /* label  */   | ID CLN stmt               {}
                | for_stmt                  {}
                | while_stmt                {}
                | do_stmt                   {}
 /* return */   | RTN exps                  {}
                | block                     {}
                | BRK                       {}
                | CTN                       {}
                | GTO ID                    {}
                | exps                      {}
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
                  stmt                      {}
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
                | simple_exp
                ;
 /* 16 */
assignment      : lval ASN cond_exp                     {}
                | lval arith_asn cond_exp %prec ASN     {}
                | lval bitwise_asn cond_exp %prec ASN   {}
                ;

 /* Left-value */
lval            : lval LBRACKET exp RBRACKET    {}
                | lval DOT ID                   {}
                | ID                            {}
                ;

arith_asn       : ADD_ASN                   {}
                | SUB_ASN                   {}
                | MUL_ASN                   {}
                | DIV_ASN                   {}
                | MOD_ASN                   {}
                ;

bitwise_asn     : B_AND_ASN                 {}
                | B_XOR_ASN                 {}
                | B_OR_ASN                  {}
                | B_R_ASN                   {}
                | B_L_ASN                   {}
                ;

 /* ? : */
cond_exp        : exps QUE exps CLN exps    {}
                ;

 /* 15 */
simple_exp      : simple_exp OR and_exp     { $$ = A_OpExp(lineno, A_orOp, $1, $2); }
                | and_exp                   { $$ = $1; }
                ;

 /* 14 */
and_exp         : and_exp AND b_or_exp      { $$ = A_OpExp(lineno, A_andOp, $1, $2); }
                | b_or_exp                  { $$ = $1; }
                ;

 /* 13 */
b_or_exp        : b_or_exp B_OR b_xor_exp   { $$ = A_OpExp(lineno, A_bOrOp, $1, $2); }
                | b_xor_exp                 { $$ = $1; }
                ;

 /* 12 */
b_xor_exp       : b_xor_exp B_XOR b_and_exp { $$ = A_OpExp(lineno, A_bXorOp, $1, $2); }
                | b_and_exp                 { $$ = $1; }
                ;

 /* 11 */
b_and_exp       : b_and_exp B_AND eq_exp    { $$ = A_OpExp(lineno, A_bAndOp, $1, $2); }
                | eq_exp                    { $$ = $1; }
                ;

 /* 10 */
eq_exp          : eq_exp EQ rel_exp                 { $$ = A_OpExp(lineno, A_eqOp, $1, $2); }
                | eq_exp NEQ rel_exp %prec EQ       { $$ = A_OpExp(lineno, A_neqOp, $1, $2); }
                | rel_exp                           { $$ = $1; }
                ;

 /* 9 */
rel_exp         : rel_exp LEQ shift_exp             { $$ = A_OpExp(lineno, A_leOp, $1, $2); }
                | rel_exp GEQ shift_exp %prec LEQ   { $$ = A_OpExp(lineno, A_geOp, $1, $2); }
                | rel_exp LE shift_exp %prec LEQ    { $$ = A_OpExp(lineno, A_ltOp, $1, $2); }
                | rel_exp GT shift_exp %prec LEQ    { $$ = A_OpExp(lineno, A_gtOp, $1, $2); }
                | shift_exp                         { $$ = $1; }
                ;

 /* 7 << >> */
shift_exp       : shift_exp B_L plus_exp            { $$ = A_OpExp(lineno, A_bLshiftOp, $1, $2); }
                | shift_exp B_R plus_exp %prec B_L  { $$ = A_OpExp(lineno, A_bRshiftOp, $1, $2); }
                | plus_exp                          { $$ = $1; }
                ;

 /* 6 */
plus_exp        : plus_exp ADD mul_exp              { $$ = A_OpExp(lineno, A_plusOp, $1, $2); }
                | plus_exp SUB mul_exp %prec SUB    { $$ = A_OpExp(lineno, A_minusOp, $1, $2); }
                | mul_exp                           { $$ = $1; }
                ;

 /* 5 */
mul_exp         : mul_exp MUL unary_exp             { $$ = A_OpExp(lineno, A_timesOp, $1, $2); }
                | mul_exp DIV unary_exp %prec MUL   { $$ = A_OpExp(lineno, A_divideOp, $1, $2); }
                | mul_exp MOD unary_exp %prec MUL   { $$ = A_OpExp(lineno, A_modOp, $1, $2); }
                | unary_exp                         { $$ = $1; }
                ;

 /* 3 */
unary_exp       : ADD post_unary %prec UNARY    { $$ = A_PreUnaryExp(lineno, A_plusOp, $1); }
                | SUB post_unary %prec UNARY    { $$ = A_PreUnaryExp(lineno, A_minusOp, $1); }
                | INC post_unary %prec UNARY    { $$ = A_PreUnaryExp(lineno, A_incOp, $1); }
                | DEC post_unary %prec UNARY    { $$ = A_PreUnaryExp(lineno, A_decOp, $1); }
                | NOT post_unary %prec UNARY    { $$ = A_PreUnaryExp(lineno, A_notOp, $1); }
                | B_NOT post_unary %prec UNARY  { $$ = A_PreUnaryExp(lineno, A_bNotOp, $1); }
                | post_unary %prec UNARY        { $$ = A_PreUnaryExp(lineno, NULL, $1); }
                ;

 /* 2 */
post_unary      : factor INC                { $$ = A_PostUnaryExp(lineno, $1, A_incOp); }
                | factor DEC %prec INC      { $$ = A_PostUnaryExp(lineno, $1, A_decOp); }
                | call                      { $$ = A_PostUnaryExp(lineno, $1, NULL); }
                | factor                    { $$ = A_PostUnaryExp(lineno, $1, NULL); }
                ;

call            : ID LPAREN args RPAREN     { $$ = A_FuncCallExp(lineno, $1, $2); }
                ;

args            : arg_list                  { $$ = A_SeqExp(lineno, $1); }
                | /* empty */               { $$ = A_NullExp(lineno); }
                ;

arg_list        : arg CMA arg_list          { $$ = A_ExpList(lineno, $1, $2);}
                | arg                       { $$ = A_ExpList(lineno, $1, NULL); }
                ;

arg             : exp                       { $$ = A_VarExp(lineno, $1); }
                ;

factor          : INT                       { $$ = A_FactorExp(lineno, $1); }
                | FLT                       { $$ = A_FactorExp(lineno, $1); }
                | STR                       { $$ = A_FactorExp(lineno, $1); }
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
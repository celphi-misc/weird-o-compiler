%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

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
simple_exp      : simple_exp OR and_exp     {}
                | and_exp
                ;

 /* 14 */
and_exp         : and_exp AND b_or_exp      {}
                | b_or_exp                  {}
                ;

 /* 13 */
b_or_exp        : b_or_exp B_OR b_xor_exp   {}
                | b_xor_exp
                ;

 /* 12 */
b_xor_exp       : b_xor_exp B_XOR b_and_exp {}
                | b_and_exp
                ;

 /* 11 */
b_and_exp       : b_and_exp B_AND eq_exp    {}
                | eq_exp
                ;

 /* 10 */
eq_exp          : eq_exp EQ rel_exp                 {}
                | eq_exp NEQ rel_exp %prec EQ       {}
                | rel_exp
                ;

 /* 9 */
rel_exp         : rel_exp LEQ shift_exp             {}
                | rel_exp GEQ shift_exp %prec LEQ   {}
                | rel_exp LE shift_exp %prec LEQ    {}
                | rel_exp GT shift_exp %prec LEQ    {}
                | shift_exp                         {}
                ;

 /* 7 << >> */
shift_exp       : shift_exp B_L plus_exp            {}
                | shift_exp B_R plus_exp %prec B_L  {}
                | plus_exp
                ;

 /* 6 */
plus_exp        : plus_exp ADD mul_exp              {}
                | plus_exp SUB mul_exp %prec SUB    {}
                | mul_exp
                ;

 /* 5 */
mul_exp         : mul_exp MUL unary_exp             {}
                | mul_exp DIV unary_exp %prec MUL   {}
                | mul_exp MOD unary_exp %prec MUL   {}
                | unary_exp
                ;

 /* 3 */
unary_exp       : ADD post_unary %prec UNARY    {}
                | SUB post_unary %prec UNARY    {}
                | INC post_unary %prec UNARY    {}
                | DEC post_unary %prec UNARY    {}
                | NOT post_unary %prec UNARY    {}
                | B_NOT post_unary %prec UNARY  {}
                | post_unary %prec UNARY        {}
                ;

 /* 2 */
post_unary      : factor INC                {}
                | factor DEC %prec INC      {}
                | call
                | factor
                ;

call            : ID LPAREN args RPAREN     {}
                ;

args            : arg_list                  {}
                | /* empty */               {}
                ;

arg_list        : arg CMA arg_list          {}
                | arg                       {}
                ;

arg             : exp                       {}
                ;

factor          : INT                       {}
                | FLT                       {}
                | STR                       {}
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
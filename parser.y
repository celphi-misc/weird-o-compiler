%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "token.h"

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

 /* TODO: %left */
%nonassoc UMINUS

%%

 /* ---- PROGRAM ---- */

program         : declarations
                ;

declarations    : declaration declarations
                | declaration
                ;

declaration     : var_dec
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

paras           : id                        {}

 /* ---- STATEMENTS ---- */

block           : LBRACE
                  statements                {}
                  RBRACE                    {}
                ;

stmts           : stmt SEM stmts            {}
                | /* empty */
                ;

stmt            : if_stmt                   {}
                | switch_stmt               {}
                | for_stmt                  {}
                | while_stmt                {}
                | do_stmt                   {}
                | declaration               {}
 /* return */   | RTN exps                  {}
                | block                     {}
                | exps                      {}
                ;

if_stmt         : IF LPAREN exps RPAREN     {}
                  stmt                      {}
                  ELS stmt                  {}
                | IF LPAREN exps RPAREN     {}
                  stmt                      {}
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
                | exp                       {}
                ;

exp             : assignment
                | simple_exp
                ;
 /* 16 */
assignment      : lval ASN cond_exp         {}
                | lval arith_asn cond_exp   {}
                | lval bitwise_asn cond_exp {}
                ;

 /* Left-value */
lval            : lval LBRACKET exp RBRACKET {}
                | lval DOT ID               {}
                | ID                        {}
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
eq_exp          : eq_exp EQ rel_exp         {}
                | eq_exp NEQ rel_exp        {}
                | rel_exp
                ;

 /* 9 */
rel_exp         : rel_exp LEQ shift_exp     {}
                | rel_exp GEQ shift_exp     {}
                | rel_exp LE shift_exp      {}
                | rel_exp GT shift_exp      {}
                | shift_exp                 {}
                ;

 /* 7 << >> */
shift_exp       : shift_exp B_L plus_exp    {}
                | shift_exp R_L plus_exp    {}
                | plus_exp
                ;

 /* 6 */
plus_exp        : plus_exp ADD mul_exp      {}
                | plus_exp SUB mul_exp      {}
                | mul_exp
                ;

 /* 5 */
mul_exp         : mul_exp MUL unary_exp     {}
                | mul_exp DIV unary_exp     {}
                | mul_exp MOD unary_exp     {}
                | unary_exp
                ;

 /* 3 */
 /* TODO */


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
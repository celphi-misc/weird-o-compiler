%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

// Curent column number
int yycolumn = 1;
// Update column number
#define YY_UPDATE yylloc->first_line = yylloc->last_line = yylineno;\
    yylloc->first_column = yycolumn; yylloc->last_column = yycolumn + yyleng - 1;\
    yycolumn += yyleng;

#define WARN_INFO(str)  printf("[WARN] %d:%d\t" str "\n", yylineno, yycolumn)
#define ERR_INFO(str)   printf("[ERR]  %d:%d\t" str "\n", yylineno, yycolumn)
#define COMM_EOF        "Unterminated comment"
#define UNKNOWN_TOKEN   "Unknown token"
%}

 /* Options */
%option bison-bridge
%option bison-locations
 /* %option header-file="scanner.h" */
%option noyywrap
%option nodefault
%option warn
%option yylineno

 /* Conditions */
%x  COMMENT

%%

 /* Comments
    Ref: Flex & Bison, p40
 */
"/*"                    { YY_UPDATE; BEGIN(COMMENT); }
<COMMENT>"*/"           { YY_UPDATE; BEGIN(INITIAL); }
<COMMENT>([^*]|\n)+|.   /* Do nothing */
<COMMENT><<EOF>>        { WARN_INFO(COMM_EOF); }
"//".*\n                /* Do nothing */
.                       { WARN_INFO(UNKNOWN_TOKEN); }

%%
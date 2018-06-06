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

 /* Lexical rules */
digit   [0-9]
letter  [a-zA-Z]
hexdig  [a-fA-F0-9]
exp     [Ee][+-]?{digit}+

%%

 /* Comments
    Ref: Flex & Bison, p40
 */
"/*"                    { YY_UPDATE; BEGIN(COMMENT); }
<COMMENT>"*/"           { YY_UPDATE; BEGIN(INITIAL); }
<COMMENT>([^*]|\n)+|.   /* Do nothing */
<COMMENT><<EOF>>        { WARN_INFO(COMM_EOF); }
"//".*\n                /* Do nothing */

 /* Type key words */
"var"                   { YY_UPDATE; return VAR; }
"function"              { YY_UPDATE; return FUN; }

 /* Return */
"return"                { YY_UPDATE; return RTN; }

 /* Control flow */
"if"                    { YY_UPDATE; return IF;  }
"else"                  { YY_UPDATE; return ELS; }
"switch"                { YY_UPDATE; return SWH; }
"case"                  { YY_UPDATE; return CAS; }
"default"               { YY_UPDATE; return DFT; }
"for"                   { YY_UPDATE; return FOR; }
"while"                 { YY_UPDATE; return WHL; }
"do"                    { YY_UPDATE; return DO;  }
"break"                 { YY_UPDATE; return BRK; }
"continue"              { YY_UPDATE; return CTN; }
"goto"                  { YY_UPDATE; return GTO; }

 /* Boolean, these are not C features */
"true"                  { YY_UPDATE; return TRU; }
"false"                 { YY_UPDATE; return FLS; }

 /* Identifier */
({letter}|"_")({letter}|{digit}|"_")*  {
                            YY_UPDATE;
                            yylval->stringVal = strdup(yytext);
                            return ID;
                        }

 /* Numerals */
0[xX]{hexdig}+          {
                            YY_UPDATE;
                            yylval->intVal = strtoll(yytext, NULL, 16);
                            return INT;
                        }
0{digit}+               {
                            YY_UPDATE;
                            yylval->intVal = strtoll(yytext, NULL, 8);
                            return INT;
                        }
{digit}+{exp}           {
                            YY_UPDATE;
                            double result;
                            sscanf(yytext, "%lf", &result);
                            yylval->floatVal = result;
                            return FLT;
                        }
{digit}*"."{digit}*exp? {
                            YY_UPDATE;
                            double result;
                            sscanf(yytext, "%lf", &result);
                            yylval->floatVal = result;
                            return FLT;
                        }
{digit}+                {
                            YY_UPDATE;
                            yylval->intVal = strtoll(yytext, NULL, 10);
                            return INT;
                        }

 /* Strings */
'(\\.|[^\\'])*'         {
                            YY_UPDATE;
                            char *result = strdup(yytext + 1);
                            result[strlen(result) - 1] = 0;
                            yylval->stringVal = result;
                            return STR;
                        }
\"(\\.|[^\\"])*\"       {
                            YY_UPDATE;
                            char *result = strdup(yytext + 1);
                            result[strlen(result) - 1] = 0;
                            yylval->stringVal = result;
                            return STR;
                        }

 /* Operators */
">>="                   { YY_UPDATE; return B_R_ASN;   }
"<<="                   { YY_UPDATE; return B_L_ASN;   }
"+="			        { YY_UPDATE; return ADD_ASN;   }
"-="			        { YY_UPDATE; return SUB_ASN;   }
"*="			        { YY_UPDATE; return MUL_ASN;   }
"/="			        { YY_UPDATE; return DIV_ASN;   }
"%="			        { YY_UPDATE; return MOD_ASN;   }
"&="			        { YY_UPDATE; return B_AND_ASN; }
"^="			        { YY_UPDATE; return B_XOR_ASN; }
"|="			        { YY_UPDATE; return B_OR_ASN;  }
">>"			        { YY_UPDATE; return B_R;       }
"<<"			        { YY_UPDATE; return B_L;       }
"++"			        { YY_UPDATE; return INC;       }
"--"			        { YY_UPDATE; return DEC;       }
 /* "->"			        { YY_UPDATE; return PTR;       } */
"&&"			        { YY_UPDATE; return AND;       }
"||"			        { YY_UPDATE; return OR;        }
"<="			        { YY_UPDATE; return LEQ;       }
">="			        { YY_UPDATE; return GEQ;       }
"=="			        { YY_UPDATE; return EQ;        }
"!="			        { YY_UPDATE; return NEQ;       }
";"			            { YY_UPDATE; return SEM;       }
("{"|"<%")		        { YY_UPDATE; return LBRACE;    }
("}"|"%>")		        { YY_UPDATE; return RBRACE;    }
","			            { YY_UPDATE; return CMA;       }
":"			            { YY_UPDATE; return CLN;       }
"="			            { YY_UPDATE; return ASN;       }
"("			            { YY_UPDATE; return LPAREN;    }
")"			            { YY_UPDATE; return RPAREN;    }
("["|"<:")		        { YY_UPDATE; return LBRACKET;  }
("]"|":>")		        { YY_UPDATE; return RBRACKET;  }
"."			            { YY_UPDATE; return DOT;       }
"&"			            { YY_UPDATE; return B_AND;     }
"!"			            { YY_UPDATE; return NOT;       }
"~"			            { YY_UPDATE; return B_NOT;     }
"-"			            { YY_UPDATE; return SUB;       }
"+"			            { YY_UPDATE; return ADD;       }
"*"			            { YY_UPDATE; return MUL;       }
"/"			            { YY_UPDATE; return DIV;       }
"%"			            { YY_UPDATE; return MOD;       }
"<"			            { YY_UPDATE; return LE;        }
">"			            { YY_UPDATE; return GT;        }
"^"			            { YY_UPDATE; return B_XOR;     }
"|"			            { YY_UPDATE; return B_OR;      }
"?"			            { YY_UPDATE; return QUE;       }

 /* Blank char */
[ \t\v\n\f]		        { YY_UPDATE; }

 /* Default */
.                       { WARN_INFO(UNKNOWN_TOKEN); }

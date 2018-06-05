%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int lineno;

%}
chars   [A-Za-z0-9]
number  [0-9]+
blank   [" "\t\r]
line    [\n]
string  \"{chars}*\"
comment \/\/.*{line}  
words   {chars}+
lparen  "("
rparen  ")"
lmpar   "["
rmpar   "]"
llpar   "{"
rlpar   "}"
plus    "+"
minus   "-"
times   "*"
div     "/"
assign  "="
equal   "=="
semico  ";"
dot     "."
other   .

%%

int         return INT;
float       return FLOAT;
char        return CHAR;
if          return IF;
else        return ELSE;
return      return RETURN;

{number}    { yylval.value = atoi(yytext); return NUMBER; }
{blank}     break;
{line}      { ++lineno; }
{string}    { yylval.value = yytext; return STRING; }
{comment}   { return COMMENT; }
{words}     { return NAME; }
{lparen}    { return LPAREN; }
{rparen}    { return RPAREN; }
{llpar}     { return LBRACE; }
{rlpar}     { return RBRACE; }
{lmpar}     { return LBRACK; }
{rmpar}     { return RBRACK; }
{plus}      { return PLUS; }
{minus}     { return MINUS; }
{times}     { return TIMES; }
{div}       { return DIV; }
{assign}    { return ASSIGN; }
{equal}     { return EQUAL; }
{semico}    { return SEMICO; }
{dot}       { return DOT; }
{other}     { fprintf(stderr, "Unknown character of index %d encountered, on line %d\n",
                *yytext, lineno); exit(1); }
%%

int yywrap(){
    return 1;
}
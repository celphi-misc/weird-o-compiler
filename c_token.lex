%{
#include <stdio.h>
#include <stdlib.h>

%}
chars   [A-Za-z0-9]
numbers [0-9]+(\.[0-9]+)?
blank   [" "\t]
line    [\n]
string  \"{chars}*\"
comment \/\/.*{line}  
words   {chars}+
lparen  \(
rparen  \)
lmpar   \[
rmpar   \]
llpar   \{
rlpar   \}
binop   [\+\-\*\/]
eq      \=
segm    \;
other   .
%%
{numbers}   { printf("NUMBER "); }
{blank}     { printf(" "); }
{line}      { printf("\n"); }
{string}    { printf("STRING "); }
{comment}   { printf("COMMENT\n"); }
{words}     { printf("WORD "); }
{lparen}    { printf("LPAREN "); }
{rparen}    { printf("RPAREN "); }
{llpar}     { printf("LLPAR "); }
{rlpar}     { printf("RLPAR "); }
{lmpar}     { printf("LMPAR "); }
{rmpar}     { printf("RMPAR "); }
{binop}     { printf("BINOP "); }
{eq}        { printf("EQ "); }
{segm}      { printf("SEGM "); }
{other}     { printf("OTHER "); }
%%

int main() {
    yylex();
}

int yywrap(){
    return 1;
}
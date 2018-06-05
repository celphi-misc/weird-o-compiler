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

%%

test: {};

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
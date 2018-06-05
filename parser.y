%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

extern FILE *yyin;
%}

%defines
%locations
%pure-parser
%error-verbose

%%

test: {};

%%

void yyerror(const char *msg)
{
    fprintf(stderr, "%d: %s\n", yyget_lineno(), msg);
}

int main(int argc, char **argv)
{
    if(argc > 1)
        yyin = fopen(argv[1], "r");
    else
        yyin = stdin;
    yyparse();
    return 0;
}
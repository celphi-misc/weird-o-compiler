%{
#include <stdio.h>
#include <stdlib.h>

int lineno = 1;
extern int yylex();
extern char* yytext;

void yyerror(char* s){
    fprintf(stderr, "Syntax Error:: line %d \n", lineno);
    fprintf(stderr, "%s\n", s);
    fprintf(stderr, "Last token: %s\n", yytext);
    exit(1);
}
%}

%token INT FLOAT IF ELSE RETURN NUMBER STRING COMMENT NAME
%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK
%token PLUS MINUS TIMES DIV ASSIGN EQUAL SEMICO DOT


#ifndef WARN_H__
#define WARN_H__

#define WARN_INFO(str)  printf("[WARN] %d:%d\t" str "\n", yylineno, yycolumn)
#define ERR_INFO(str)   printf("[ERR]  %d:%d\t" str "\n", yylineno, yycolumn)
#define COMM_EOF        "Unterminated comment"
#define UNKNOWN_TOKEN   "Unknown token"

#endif
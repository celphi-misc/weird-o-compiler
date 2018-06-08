#ifndef WARN_H__
#define WARN_H__

#define WARN_INFO(str)  fprintf(stderr, "[WARN] %d:%d\t" str "\n", yylineno, yycolumn)
#define ERR_INFO(str)   fprintf(stderr, "[ERR]  %d:%d\t" str "\n", yylineno, yycolumn)
#define COMM_EOF        "Unterminated comment"
#define UNKNOWN_TOKEN   "Unknown token"

#endif
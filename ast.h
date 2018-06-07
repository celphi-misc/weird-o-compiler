#ifndef AST_H__
#define AST_H__

typedef int Pos;

typedef enum {
    ADD_OP, SUB_OP, INC_OP, DEC_OP, NOT_OP, B_NOT_OP,
    MUL_OP, DIV_OP, MOD_OP,
    B_L_OP, B_R_OP,
    LEQ_OP, GEQ_OP, LE_OP, GT_OP,
} Op;

typedef struct Node_ {
    enum
    {
        A_NUL, A_INT, A_FLOAT, A_BOOLEAN, A_STRING,
        A_PREUNARY, A_POSTUNARY, A_BINARY, A_TRINARY,
    } kind;
    Pos pos;
    union {
        int booleanVal;
        long long intVal;
        double floatVal;
        char *stringVal;
        struct { Op op; struct Node_* exp; } preUnaryExp;
        struct { struct Node_* exp; Op op; } postUnaryExp;
        struct { struct Node_* left; Op op; struct Node_* right; } binaryExp;
        struct { struct Node_* test; struct Node_* ift; struct Node_* iff; } trinaryExp;
    } u;
} Node;

typedef Node* pNode;

pNode A_NullExp(Pos pos);
pNode A_BooleanExp(Pos pos, int val);
pNode A_IntExp(Pos pos, long long val);
pNode A_FloatExp(Pos pos, double val);
pNode A_StringExp(Pos pos, char* val);
pNode A_PreUnaryExp(Pos pos, Op op, pNode exp);
pNode A_PostUnaryExp(Pos pos, Op op, pNode exp);
pNode A_BinnaryExp(Pos pos, Op op, pNode exp1, pNode exp2);
pNode A_TrinaryExp(Pos pos, Op op, pNode exp1, pNode exp2, pNode exp3);

#endif
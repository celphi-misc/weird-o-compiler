#ifndef AST_H__
#define AST_H__

typedef int Pos;

typedef enum {
    ADD_OP, SUB_OP, INC_OP, DEC_OP, NOT_OP, B_NOT_OP,
    MUL_OP, DIV_OP, MOD_OP,
    B_L_OP, B_R_OP,
    LEQ_OP, GEQ_OP, LE_OP, GT_OP, 
    EQ_OP, NEQ_OP,
    B_AND_OP, B_XOR_OP, B_OR_OP,
    AND_OP, OR_OP, DOT_OP, BRACKET_OP,
} Op;

typedef struct Node_ {
    enum
    {
        A_NUL, A_INT, A_FLOAT, A_BOOLEAN, A_STRING,
        A_PREUNARY, A_POSTUNARY, A_BINARY, A_TRINARY,
        A_ASSIGN, A_ID, A_CALL, A_EXPS, A_VOID,
        A_DO, A_WHILE, A_FOR,
        A_IF, A_STMTS, A_EMPTY_STMT,
        A_SWITCH, A_CASE, A_DEFAULT,
        A_RETURN, A_BREAK, A_CONTINUE, A_GOTO, A_LABEL, 
    } kind;
    Pos pos;
    union {
        int booleanVal;
        long long intVal;
        double floatVal;
        char *stringVal;
        char *name;
        struct { struct Node_* exp; Op op; } preUnaryExp;
        struct { struct Node_* exp; Op op; } postUnaryExp;
        struct { struct Node_* left; Op op; struct Node_* right; } binaryExp;
        struct { struct Node_* test; struct Node_* ift; struct Node_* iff; } trinaryExp;
        struct { struct Node_* left; struct Node_* right; } assignExp;
        struct { struct Node_* left; struct Node_* right; } dotExp;
        struct { struct Node_* head; struct Node_* arg_list; } callExp;
        struct { struct Node_* left; struct Node_* right; } exps;
        struct { struct Node_* left; struct Node_* right; } stmtsExp;
        struct { struct Node_* test; struct Node_* body; } doExp;
        struct { struct Node_* test; struct Node_* body; } whileExp;
        struct { struct Node_* start; struct Node_* test; struct Node_* incr; struct Node_* body; } forExp;
        struct { struct Node_* test; struct Node_* if_clause; struct Node_* else_clause; } ifExp;
        struct { struct Node_* exp; struct Node_* case_clause; } switchExp;
        struct { struct Node_* exp; struct Node_* stmts; } caseExp;
        struct { struct Node_* stmts; } defaultExp;
        struct { struct Node_* exp; } returnExp;
        struct { struct Node_* id; } gotoExp;
        struct { struct Node_* id; } labelExp;
    } u;
} Node;

typedef Node* pNode;

pNode A_StmtsExp(Pos pos, pNode left, pNode right); // stmt; stmt
pNode A_EmptyStmtExp(Pos pos);                      // ;
pNode A_Exps(Pos pos, pNode left, pNode right);     // exp, exp
pNode A_VoidExp(Pos pos);                           // Used for 0 arguments
pNode A_NullExp(Pos pos);                           // null
pNode A_BooleanExp(Pos pos, int val);               // true false
pNode A_IntExp(Pos pos, long long val);
pNode A_FloatExp(Pos pos, double val);
pNode A_StringExp(Pos pos, char* val);
pNode A_PreUnaryExp(Pos pos, Op op, pNode exp);
pNode A_PostUnaryExp(Pos pos, Op op, pNode exp);
pNode A_BinaryExp(Pos pos, Op op, pNode exp1, pNode exp2);
pNode A_TrinaryExp(Pos pos, pNode exp1, pNode exp2, pNode exp3);
pNode A_AssignExp(Pos pos, pNode left, pNode right);
pNode A_IdExp(Pos pos, char* val);
pNode A_CallExp(Pos pos, pNode head, pNode arg_list);
pNode A_DoExp(Pos pos, pNode test, pNode body);
pNode A_WhileExp(Pos pos, pNode test, pNode body);
pNode A_ForExp(Pos pos, pNode start, pNode test, pNode incr, pNode body);
pNode A_IfExp(Pos pos, pNode test, pNode if_clause, pNode else_clause);
pNode A_SwitchExp(Pos pos, pNode exp, pNode case_clause);
pNode A_CaseExp(Pos pos, pNode exp, pNode stmts);
pNode A_DefaultExp(Pos pos, pNode stmts);
pNode A_ReturnExp(Pos pos, pNode exp);
pNode A_BreakExp(Pos pos);
pNode A_ContinueExp(Pos pos);
pNode A_GotoExp(Pos pos, pNode id);
pNode A_LabelExp(Pos pos, pNode id);

#endif
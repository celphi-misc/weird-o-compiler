#ifndef ABSYN_TYPES_H
#define ABSYN_TYPES_H

typedef int A_pos;
typedef struct symbol_* A_symbol;
struct symbol_{
    char* name;
    A_symbol next;
};

// suppose the variables has no type
typedef struct var_* A_var;
struct var_{
    A_pos pos;
    A_symbol name;
};

// function return
// if else
// for while do break continue goto
// binop assign
// constants: number, char, string
typedef struct exp_* A_exp;
struct exp_{
    enum{ A_callExp, A_returnExp, A_ifExp, 
        A_forExp, A_whileExp, A_breakExp, A_continueExp, A_gotoExp, 
        A_switchExp, A_caseExp, A_defaultExp,
        A_opExp, A_assignExp, A_constExp} kind;
    A_pos pos;
    union {
        struct { A_symbol name; A_expList list; } call;
        struct { A_var var; } reternn;
        struct { A_exp test, then, elsee; } iff;
        struct { A_exp test, body; } whilee;
        struct { A_exp init, escape, iter; } forr;
        // break, continue: no content
        struct { A_label label; } gotoo;
        struct { A_var switched; A_exp casee, defaultt; } switchh;
        struct { A_exp constExp, body; } casee;
        struct { A_exp body; } defaultt;
        struct { A_op operatorr; A_exp left, right; } op;
        struct { A_var left; A_exp right; } assign;
        struct { A_value value; } constt;
    } u;
};

// expression list
typedef struct expList_* A_expList;
struct expList_{
    A_exp exp;
    A_expList next;
};

// goto label
typedef struct label_* A_label;
struct label_{
    A_pos pos;
    A_symbol label;
};

// binary operations
typedef enum {A_plusOp, A_minusOp, A_timesOp, A_divideOp,
        A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp} A_op;

// const values
typedef enum { A_num, A_char, A_string } A_value;

// fields

#endif
#ifndef ABSYN_TYPES_H
#define ABSYN_TYPES_H

// line position
typedef int A_pos;
typedef struct function_* A_func;
struct function_ {
    char* name;
    A_pos pos;
    A_expList params;
    A_exp block;
    A_func next;
};
A_func funcList;

typedef struct var_* A_var;
struct var_ {
    char* name;
    A_pos pos;
    A_exp init;
    A_var next;
};
A_var varList;

typedef struct field_id_* A_field;
struct field_id_{
    int id;
    A_pos pos;
};
int fieldNum = 0;

typedef struct label_* A_label;
struct label_ {
    char* name;
    A_pos pos;
    A_label next;
};
A_label labelList;

// empty-expression
// sequence-of-exps block-of-exps
// variable-expression variable-declaration
// function-dec function-call return
// if-else
// for while do break continue label goto
// switch case default
// binop assign
// unaryOp(pre, post)
// factor: int, float, string
typedef struct exp_* A_exp;
struct exp_{
    enum{ A_nullExp, A_seqExp, A_blockExp, A_varExp, A_varDecExp,
        A_funcDecExp, A_funcCallExp, A_returnExp, A_ifExp, 
        A_forExp, A_whileExp, A_doExp, A_breakExp, A_continueExp, A_labelExp, A_gotoExp, 
        A_switchExp, A_caseExp, A_defaultExp,
        A_opExp, A_assignExp, A_preUnaryExp, A_postUnaryExp,
        A_factorExp} kind;
    A_pos pos;
    union {
        // null exp: no content
        struct { A_expList list; } seq;
        struct { A_field field; A_expList list; } block;
        struct { char* name; } var;
        struct { A_var var; } varDec;
        struct { A_func func; } funcDec;
        struct { char* name; A_expList params; } call;
        struct { A_exp returnn; } reternn;
        struct { A_exp test, then, elsee; } iff;
        struct { A_exp init, escape, iter, block; } forr;
        struct { A_exp test, block; } whilee;
        struct { A_exp block, test; } doo;
        // break, continue: no content
        struct { A_label label; } label;
        struct { char* name; } gotoo;
        struct { A_exp switched; A_exp casee, defaultt; } switchh;
        struct { A_exp factor, block; } casee;
        struct { A_exp block; } defaultt;
        struct { A_op operatorr; A_exp left, right; } op;
        struct { A_op operatorr; char* left; A_exp right; } assign;
        struct { A_op operatorr; A_exp right; } preUnaryExp;
        struct { A_exp left; A_op operatorr; } postUnaryExp;
        struct { A_value value; } factor;
    } u;
};

// expression list
typedef struct expList_* A_expList;
struct expList_{
    A_exp exp;
    A_expList next;
};

// operators
// + - * / %
// && || 
// & | ^
// = != > >= < <=
// += -= *= /= %=
// &= ^= |= << >>
// ++ -- ! ~
typedef enum {A_plusOp, A_minusOp, A_timesOp, A_divideOp, A_modOp, 
        A_andOP, A_orOp, A_bAndOp, A_bOrOp, A_bXorOp, 
        A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp,
        A_addAsnOp, A_subAsnOp, A_mulAsnOp, A_divAsnOp, A_modAsnOp,
        A_bAndAsnOp, A_bXorAsnOp, A_bOrAsnOp, A_bRshiftOp, A_bLshiftOp,
        A_incOp, A_decOp, A_notOp, A_bNotOp } A_op;

// const values
typedef enum { A_int, A_float, A_string } A_value;

#endif
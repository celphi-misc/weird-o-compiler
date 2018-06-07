#ifndef ABSYNTAX_H_
#define ABSYNTAX_H_

#include "absyn_types.h"

// interfaces for reduction

A_exp   A_NullExp(A_pos pos);
A_exp   A_SeqExp(A_pos pos, A_expList list);
A_exp   A_BlockExp(A_pos pos, A_field field, A_expList list);
A_exp   A_VarExp(A_pos pos, char* name);
A_exp   A_VarDecExp(A_pos pos, A_var var);
A_exp   A_FuncDecExp(A_pos pos, A_func func);
A_exp   A_FuncCallExp(A_pos pos, char* name, A_expList params);
A_exp   A_ReturnExp(A_pos pos, A_exp returnn);
A_exp   A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee);
A_exp   A_ForExp(A_pos pos, A_exp init, A_exp escape, A_exp iter, A_exp block);
A_exp   A_WhileExp(A_pos pos, A_exp test, A_exp block);
A_exp   A_DoExp(A_pos pos, A_exp block, A_exp test);
A_exp   A_BreakExp(A_pos pos);
A_exp   A_ContinueExp(A_pos pos);
A_exp   A_LabelExp(A_pos pos, A_label label);
A_exp   A_GotoExp(A_pos pos, char* name);
A_exp   A_SwitchExp(A_pos pos, A_exp switched, A_exp casee, A_exp defaultt);
A_exp   A_CaseExp(A_pos pos, A_exp factor, A_exp block);
A_exp   A_DefaultExp(A_pos pos, A_exp block);
A_exp   A_OpExp(A_pos pos, A_op operatorr, A_exp left, A_exp right);
A_exp   A_AssignExp(A_pos pos, A_op operatorr, char* left, A_exp right);
A_exp   A_PreUnaryExp(A_pos pos, A_op operatorr, A_exp right);
A_exp   A_PostUnaryExp(A_pos pos, A_exp left, A_op operatorr);
A_exp   A_FactorExp(A_pos pos, A_value value);

A_expList   A_ExpList(A_pos pos, A_exp exp, A_expList list);
A_var   A_Var(A_pos pos, char* name, A_exp init);
A_field A_Field(A_pos pos);
A_func  A_FuncDec(A_pos pos, char* name, A_expList params, A_exp block);
A_label A_Label(A_pos pos, char* name);
// Consider how to construct tree in one-pass?
// otherwise define structure for offline tree construction

// initial structure pointers
void init();

#endif
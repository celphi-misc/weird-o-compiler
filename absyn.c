#include "absyn.h"
#include "absyn_util.h"

A_exp   A_NullExp(A_pos pos){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_nullExp;
    p->pos = pos;
    return p;
}

A_exp   A_SeqExp(A_pos pos, A_expList list){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_seqExp;
    p->pos = pos;
    p->u.seq.list = list;
    return p;
}

A_exp   A_BlockExp(A_pos pos, A_field field, A_expList list){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_blockExp;
    p->pos = pos;
    p->u.block.field = field;
    p->u.block.list = list;
    return p;
}

A_exp   A_VarExp(A_pos pos, char* name){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_varExp;
    p->pos = pos;
    p->u.var.name = name;
    return p;
}

A_exp   A_VarDecExp(A_pos pos, A_var var){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_varDecExp;
    p->pos = pos;
    p->u.varDec.var = var;
    return p;
}

A_exp   A_FuncDecExp(A_pos pos, A_func func){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_funcDecExp;
    p->pos = pos;
    p->u.funcDec.func = func;
    return p;
}

A_exp   A_FuncCallExp(A_pos pos, char* name, A_expList params){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_funcCallExp;
    p->pos = pos;
    p->u.call.name = name;
    p->u.call.params = params;
    return p;
}

A_exp   A_ReturnExp(A_pos pos, A_exp returnn){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_returnExp;
    p->pos = pos;
    p->u.reternn.returnn = returnn;
    return p;
}

A_exp   A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_ifExp;
    p->pos = pos;
    p->u.iff.test =test;
    p->u.iff.then = then;
    p->u.iff.elsee = elsee;
    return p;
}

A_exp   A_ForExp(A_pos pos, A_exp init, A_exp escape, A_exp iter, A_exp block){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_forExp;
    p->pos = pos;
    p->u.forr.init = init;
    p->u.forr.escape = escape;
    p->u.forr.iter = iter;
    p->u.forr.block = block;
    return p;
}

A_exp   A_WhileExp(A_pos pos, A_exp test, A_exp block){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_whileExp;
    p->pos = pos;
    p->u.whilee.test = test;
    p->u.whilee.block = block;
    return p;
}

A_exp   A_DoExp(A_pos pos, A_exp block, A_exp test){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_doExp;
    p->pos = pos;
    p->u.doo.block = block;
    p->u.doo.test = test;
    return p;
}

A_exp   A_BreakExp(A_pos pos){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_breakExp;
    p->pos = pos;
    return p;
}

A_exp   A_ContinueExp(A_pos pos){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_continueExp;
    p->pos = pos;
    return p;
}

A_exp   A_LabelExp(A_pos pos, A_label label){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_labelExp;
    p->pos = pos;
    p->u.label.label = label;
    return p;
}

A_exp   A_GotoExp(A_pos pos, char* name){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_gotoExp;
    p->pos = pos ;
    p->u.gotoo.name = name;
    return p;
}

A_exp   A_SwitchExp(A_pos pos, A_exp switched, A_exp casee, A_exp defaultt){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_switchExp;
    p->pos = pos;
    p->u.switchh.switched = switched;
    p->u.switchh.casee = casee;
    p->u.switchh.defaultt = defaultt;
    return p;
}

A_exp   A_CaseExp(A_pos pos, A_exp factor, A_exp block){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_caseExp;
    p->pos = pos;
    p->u.casee.factor = factor;
    p->u.casee.block = block;
    return p;
}

A_exp   A_DefaultExp(A_pos pos, A_exp block){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_defaultExp;
    p->pos = pos;
    p->u.defaultt.block = block;
    return p;
}

A_exp   A_OpExp(A_pos pos, A_op operatorr, A_exp left, A_exp right){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_opExp;
    p->pos = pos;
    p->u.op.operatorr = operatorr;
    p->u.op.left = left;
    p->u.op.right = right;
    return p;
}

A_exp   A_AssignExp(A_pos pos, A_op operatorr, A_var left, A_exp right){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_assignExp;
    p->pos = pos;
    p->u.assign.operatorr = operatorr;
    p->u.assign.left = left;
    p->u.assign.right = right;
    return p;
}

A_exp   A_PreUnaryExp(A_pos pos, A_op operatorr, A_exp right){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_preUnaryExp;
    p->pos = pos;
    p->u.preUnaryExp.operatorr = operatorr;
    p->u.preUnaryExp.right = right;
    return p;
}

A_exp   A_PostUnaryExp(A_pos pos, A_exp left, A_op operatorr){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_postUnaryExp;
    p->pos =pos;
    p->u.postUnaryExp.left = left;
    p->u.postUnaryExp.operatorr = operatorr;
    return p;
}

A_exp   A_FactorExp(A_pos pos, A_value value){
    A_exp p = checked_malloc(sizeof(*p));
    p->kind = A_factorExp;
    p->pos = pos;
    p->u.factor.value = value;
    return p;
}


A_expList   A_ExpList(A_pos pos, A_exp exp, A_expList list){
    A_expList p = checked_malloc(sizeof(*p));
    p->exp = exp;
    p->next = list;
    return p;
}

A_var   A_Var(A_pos pos, char* name, A_exp init){
    A_var p = lookupVar(name);
    if(p){
        error(ERROR_VAR_REDEFINE);
        exit(1);
    }
    A_var p = checked_malloc(sizeof(*p));
    p->name = name;
    p->pos = pos;
    p->init = init;
    insertVar(varList, p); // p->next
    return p;
}

A_field A_Field(A_pos pos){
    A_field p = checked_malloc(sizeof(*p));
    p->pos = pos;
    p->id = fieldNum;
    fieldNum++;
    return p;
}

A_func  A_FuncDec(A_pos pos, char* name, A_expList params, A_exp block){
    A_func p = lookupFunc(name);
    if(p) {
        error(ERROR_FUNC_REDEFINE);// todo
        exit(1);
    }
    A_func p = checked_malloc(sizeof(*p));
    p->name = name;
    p->params = params;
    p->block = block;
    p->pos = pos;
    insertFunc(funcList, p); // p->next
    return p;
}

A_label A_Label(A_pos pos, char* name){
    A_label p = lookupLabel(name);
    if(p) {
        error(ERROR_LABEL_REDEFINE); // todo
        exit(1);
    }
    A_label p = checked_malloc(sizeof(*p));
    p->name = name;
    p->pos = pos;
    insertLabel(labelList, p); // p->next
    return p;
}

void init(){
    varList = NULL;
    labelList = NULL;
    funcList = NULL;
    fieldNum = 0;
}
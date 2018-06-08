#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

pNode A_NullExp(Pos pos)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_NUL;
    result->pos = pos;
    return result;
}

pNode A_BooleanExp(Pos pos, int val)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_BOOLEAN;
    result->pos = pos;
    result->u.booleanVal = val ? 1 : 0;
    return result;
}

pNode A_IntExp(Pos pos, long long val)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_INT;
    result->pos = pos;
    result->u.intVal = val;
    return result;
}

pNode A_FloatExp(Pos pos, double val)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_FLOAT;
    result->pos = pos;
    result->u.floatVal = val;
    return result;
}

pNode A_StringExp(Pos pos, char* val)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_STRING;
    result->pos = pos;
    result->u.stringVal = val; 
    return result;
}

pNode A_PreUnaryExp(Pos pos, Op op, pNode exp)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_PREUNARY;
    result->pos = pos;
    result->u.preUnaryExp.op = op;
    result->u.preUnaryExp.exp = exp;
    return result;
}

pNode A_PostUnaryExp(Pos pos, Op op, pNode exp)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_POSTUNARY;
    result->pos = pos;
    result->u.postUnaryExp.op = op;
    result->u.postUnaryExp.exp = exp;
    return result;
}

pNode A_BinaryExp(Pos pos, Op op, pNode exp1, pNode exp2)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_BINARY;
    result->pos = pos;
    result->u.binaryExp.left = exp1;
    result->u.binaryExp.op = op;
    result->u.binaryExp.right = exp2;
    return result;
}

pNode A_TrinaryExp(Pos pos, pNode exp1, pNode exp2, pNode exp3)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_TRINARY;
    result->pos = pos;
    result->u.trinaryExp.test = exp1;
    result->u.trinaryExp.ift = exp2;
    result->u.trinaryExp.iff = exp3;
    return result;
}

pNode A_AssignExp(Pos pos, pNode left, pNode right)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_ASSIGN;
    result->pos = pos;
    result->u.assignExp.left = left;
    result->u.assignExp.right = right;
    return result;
}

pNode A_IdExp(Pos pos, char* val)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_ID;
    result->pos = pos;
    result->u.name = val; 
    return result;
}

pNode A_CallExp(Pos pos, pNode head, pNode arg_list)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_CALL;
    result->pos = pos;
    result->u.callExp.head = head;
    result->u.callExp.arg_list = arg_list;
    return result;
}

pNode A_Exps(Pos pos, pNode left, pNode right)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_EXPS;
    result->pos = pos;
    result->u.exps.left = left;
    result->u.exps.right = right;
    return result;
}

pNode A_VoidExp(Pos pos)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_VOID;
    result->pos = pos;
    return result;
}

pNode A_DoExp(Pos pos, pNode test, pNode body)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_DO;
    result->pos = pos;
    result->u.doExp.test = test;
    result->u.doExp.body = body;
    return result;
}

pNode A_WhileExp(Pos pos, pNode test, pNode body)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_WHILE;
    result->pos = pos;
    result->u.whileExp.test = test;
    result->u.whileExp.body = body;
    return result;
}

pNode A_ForExp(Pos pos, pNode start, pNode test, pNode incr, pNode body)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_FOR;
    result->pos = pos;
    result->u.forExp.start = start;
    result->u.forExp.test = test;
    result->u.forExp.incr = incr;
    result->u.forExp.body = body;
    return result;
}

pNode A_IfExp(Pos pos, pNode test, pNode if_clause, pNode else_clause)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_IF;
    result->pos = pos;
    result->u.ifExp.test = test;
    result->u.ifExp.if_clause = if_clause;
    result->u.ifExp.else_clause = else_clause;
    return result;
}

pNode A_EmptyStmtExp(Pos pos)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_EMPTY_STMT;
    result->pos = pos;
    return result;
}

pNode A_StmtsExp(Pos pos, pNode left, pNode right)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_STMTS;
    result->pos = pos;
    result->u.stmtsExp.left = left;
    result->u.stmtsExp.right = right;
    return result;
}

pNode A_SwitchExp(Pos pos, pNode exp, pNode case_clause)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_SWITCH;
    result->pos = pos;
    result->u.switchExp.exp = exp;
    result->u.switchExp.case_clause = case_clause;
    return result;
}

pNode A_CaseExp(Pos pos, pNode exp, pNode stmts)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_CASE;
    result->pos = pos;
    result->u.caseExp.exp = exp;
    result->u.caseExp.stmts = stmts;
    return result;
}

pNode A_DefaultExp(Pos pos, pNode stmts)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_DEFAULT;
    result->pos = pos;
    result->u.defaultExp.stmts = stmts;
    return result;
}

pNode A_ReturnExp(Pos pos, pNode exp)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_RETURN;
    result->pos = pos;
    result->u.returnExp.exp = exp;
    return result;
}

pNode A_BreakExp(Pos pos)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_BREAK;
    result->pos = pos;
    return result;
}

pNode A_ContinueExp(Pos pos)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_CONTINUE;
    result->pos = pos;
    return result;
}

pNode A_LabelExp(Pos pos, pNode id)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_LABEL;
    result->pos = pos;
    result->u.labelExp.id = id;
    return result;
}

pNode A_GotoExp(Pos pos, pNode id)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_GOTO;
    result->pos = pos;
    result->u.gotoExp.id = id;
    return result;
}

pNode A_BlockExp(Pos pos, pNode stmts)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_BLOCK;
    result->pos = pos;
    result->u.blockExp.stmts = stmts;
    return result;
}

pNode A_FunctionExp(Pos pos, pNode id, pNode para_list, pNode block)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_FUNCTION;
    result->pos = pos;
    result->u.functionExp.id = id;
    result->u.functionExp.para_list = para_list;
    result->u.functionExp.block = block;
    return result;
}

pNode A_VarInitExp(Pos pos, pNode id, pNode init)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_VAR_INIT;
    result->pos = pos;
    result->u.varInitExp.id = id;
    result->u.varInitExp.init = init;
    return result;
}

pNode A_VarDecExp(Pos pos, pNode id)
{
    pNode result = (pNode)malloc(sizeof(Node));
    result->kind = A_VAR_DEC;
    result->pos = pos;
    result->u.varDecExp.id = id;
    return result;
}
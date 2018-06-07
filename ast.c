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
    result->kind = A_STRING;
    result->pos = pos;
    result->u.name = val; 
    return result;
}
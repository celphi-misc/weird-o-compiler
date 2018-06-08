#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_print.h"

char buffer[0xfff2];

const char *getOpName(Op op)
{
    switch(op)
    {
        case ADD_OP: return "add";
        case SUB_OP: return "sub";
        case INC_OP: return "inc";
        case DEC_OP: return "dec";
        case NOT_OP: return "not";
        case B_NOT_OP: return "bnot";
        case MUL_OP: return "mul";
        case DIV_OP: return "div";
        case MOD_OP: return "mod";
        case B_L_OP: return "bleft";
        case B_R_OP: return "bright";
        case LEQ_OP: return "leq";
        case GEQ_OP: return "geq";
        case LE_OP: return "le";
        case GT_OP: return "gt";
        case EQ_OP: return "eq";
        case NEQ_OP: return "neq";
        case B_AND_OP: return "band";
        case B_XOR_OP: return "bxor";
        case B_OR_OP: return "bor";
        case AND_OP: return "and";
        case OR_OP: return "or";
        case DOT_OP: return "dot";
        case BRACKET_OP: return "bracket";
        default: return "[error found in function getOpName]";
    }
}

static int printBuffer(pNode node, char *buf)
{
    char *old_buf = buf;
    buf += sprintf(buf, "{");
    if(!node)
    {
        fprintf(stderr, "Error met when printing AST tree\n");
        goto end;
    }
    switch(node->kind)
    {
        case A_NUL:
            buf += sprintf(buf, "\"node\":\"null\""); break;
        case A_INT:
            buf += sprintf(buf, "\"node\":\"int\",\"val\":%lld", node->u.intVal); break;
        case A_FLOAT:
            buf += sprintf(buf, "\"node\":\"float\",\"val\":%lf", node->u.floatVal); break;
        case A_BOOLEAN:
            buf += sprintf(buf, "\"node\":\"boolean\",\"val\":%s", node->u.booleanVal?"true":"false"); break;
        case A_STRING:
            buf += sprintf(buf, "\"node\": \"string\",\"val\":\"%s\"", node->u.stringVal); break;
        case A_PREUNARY:
            buf += sprintf(buf, "\"node\":\"preunary\",\"op\":\"%s\",\"exp\":", getOpName(node->u.preUnaryExp.op));
            buf += printBuffer(node->u.preUnaryExp.exp, buf);
            break;
        case A_POSTUNARY:
            buf += sprintf(buf, "\"node\":\"postunary\",\"exp\":");
            buf += printBuffer(node->u.postUnaryExp.exp, buf);
            buf += sprintf(buf, ",\"op\":\"%s\"", getOpName(node->u.postUnaryExp.op));
            break;
        case A_BINARY:
            buf += sprintf(buf, "\"node\":\"binary\",\"left\":");
            buf += printBuffer(node->u.binaryExp.left, buf);
            buf += sprintf(buf, ",\"op\":\"%s\",\"right\":", getOpName(node->u.binaryExp.op));
            buf += printBuffer(node->u.binaryExp.right, buf);
            break;
        case A_TRINARY:
            buf += sprintf(buf, "\"node\":\"trinary\",\"test\":");
            buf += printBuffer(node->u.trinaryExp.test, buf);
            buf += sprintf(buf, ",\"ift\":");
            buf += printBuffer(node->u.trinaryExp.ift, buf);
            buf += sprintf(buf, ",\"iff\":");
            buf += printBuffer(node->u.trinaryExp.iff, buf);
            break;
        case A_ASSIGN:
            buf += sprintf(buf, "\"node\":\"assign\",\"left\":");
            buf += printBuffer(node->u.assignExp.left, buf);
            buf += sprintf(buf, ",\"right\":");
            buf += printBuffer(node->u.assignExp.right, buf);
            break;
        case A_ID:
            buf += sprintf(buf, "\"node\":\"id\",\"name\":\"%s\"", node->u.name); break;
        case A_CALL:
            buf += sprintf(buf, "\"node\":\"call\",\"head\":");
            buf += printBuffer(node->u.callExp.head, buf);
            buf += sprintf(buf, ",\"arg_list\":");
            buf += printBuffer(node->u.callExp.arg_list, buf);
            break;
        case A_EXPS:
            buf += sprintf(buf, "\"node\":\"exps\",\"left\":");
            buf += printBuffer(node->u.exps.left, buf);
            buf += sprintf(buf, ",\"right\":");
            buf += printBuffer(node->u.exps.right, buf);
            break;
        case A_VOID:
            buf += sprintf(buf, "\"node\":\"void\""); break;
        case A_DO:
            buf += sprintf(buf, "\"node\":\"do\",\"test\":");
            buf += printBuffer(node->u.doExp.test, buf);
            buf += sprintf(buf, ",\"body\":");
            buf += printBuffer(node->u.doExp.body, buf);
            break;
        case A_WHILE:
            buf += sprintf(buf, "\"node\":\"while\",\"test\":");
            buf += printBuffer(node->u.whileExp.test, buf);
            buf += sprintf(buf, ",\"body\":");
            buf += printBuffer(node->u.whileExp.body, buf);
            break;
        case A_FOR:
            buf += sprintf(buf, "\"node\":\"for\",\"start\":");
            buf += printBuffer(node->u.forExp.start, buf);
            buf += sprintf(buf, ",\"test\":");
            buf += printBuffer(node->u.forExp.test, buf);
            buf += sprintf(buf, ",\"incr\":");
            buf += printBuffer(node->u.forExp.incr, buf);
            buf += sprintf(buf, ",\"body\":");
            buf += printBuffer(node->u.forExp.body, buf);
            break;
        case A_IF:
            buf += sprintf(buf, "\"node\":\"if\",\"test\":");
            buf += printBuffer(node->u.ifExp.test, buf);
            buf += sprintf(buf, ",\"if_clause\":");
            buf += printBuffer(node->u.ifExp.if_clause, buf);
            buf += sprintf(buf, ",\"else_clase\":");
            buf += printBuffer(node->u.ifExp.else_clause, buf);
            break;
        case A_STMTS:
            buf += sprintf(buf, "\"node\":\"stmts\",\"left\":");
            buf += printBuffer(node->u.stmtsExp.left, buf);
            buf += sprintf(buf, ",\"right\":");
            buf += printBuffer(node->u.stmtsExp.right, buf);
            break;
        case A_EMPTY_STMT:
            buf += sprintf(buf, "\"node\":\"empty_stmt\""); break;
        case A_SWITCH:
            buf += sprintf(buf, "\"node\":\"switch\",\"exp\":");
            buf += printBuffer(node->u.switchExp.exp, buf);
            buf += sprintf(buf, ",\"case_clause\":");
            buf += printBuffer(node->u.switchExp.case_clause, buf);
            break;
        case A_CASE:
            buf += sprintf(buf, "\"node\":\"case\",\"exp\":");
            buf += printBuffer(node->u.caseExp.exp, buf);
            buf += sprintf(buf, ",\"stmts\":");
            buf += printBuffer(node->u.caseExp.stmts, buf);
            break;
        case A_DEFAULT:
            buf += sprintf(buf, "\"node\":\"default\",\"stmts\":");
            buf += printBuffer(node->u.defaultExp.stmts, buf);
            break;
        case A_RETURN:
            buf += sprintf(buf, "\"node\":\"return\",\"exp\":");
            buf += printBuffer(node->u.returnExp.exp, buf);
            break;
        case A_BREAK:
            buf += sprintf(buf, "\"node\":\"break\""); break;
        case A_CONTINUE:
            buf += sprintf(buf, "\"node\":\"continue\""); break;
        case A_GOTO:
            buf += sprintf(buf, "\"node\":\"goto\",\"id\":");
            buf += printBuffer(node->u.gotoExp.id, buf);
            break;
        case A_LABEL:
            buf += sprintf(buf, "\"node\":\"label\",\"id\":");
            buf += printBuffer(node->u.labelExp.id, buf);
            break;
        case A_BLOCK:
            buf += sprintf(buf, "\"node\":\"block\",\"stmts\":");
            buf += printBuffer(node->u.blockExp.stmts, buf);
            break;
        case A_FUNCTION:
            buf += sprintf(buf, "\"node\":\"function\",\"id\":");
            buf += printBuffer(node->u.functionExp.id, buf);
            buf += sprintf(buf, ",\"para_list\":");
            buf += printBuffer(node->u.functionExp.para_list, buf);
            buf += sprintf(buf, ",\"block\":");
            buf += printBuffer(node->u.functionExp.block, buf);
            break;
        case A_VAR_INIT:
            buf += sprintf(buf, "\"node\":\"var_init\",\"id\":");
            buf += printBuffer(node->u.varInitExp.id, buf);
            buf += sprintf(buf, ",\"init\":");
            buf += printBuffer(node->u.varInitExp.init, buf);
            break;
        case A_VAR_DEC:
            buf += sprintf(buf, "\"node\":\"var_dec\",\"id\":");
            buf += printBuffer(node->u.varDecExp.id, buf);
            break;
    }
    end:
    buf += sprintf(buf, "}");
    return buf - old_buf;
}

char *createAstJsonStr(pNode astRoot)
{
    buffer[0] = 0;
    printBuffer(astRoot, buffer);
    char *result = (char*)malloc(strlen(buffer) + 1);
    strcpy(result, buffer);
    return result;
}
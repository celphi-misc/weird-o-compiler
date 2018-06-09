#ifndef IR_TREE_H__
#define IR_TREE_H__

// #define _DEBUG

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;
typedef int boolean;
#define TRUE    1
#define FALSE   0

// IR tree node structure
typedef struct ir_node_t* TreeNode;
struct ir_node_t{
    string name;
    int pos;
    int numOfChild;
    TreeNode* childs;
};

// label list structure
typedef struct label_t* Label;
struct label_t{
    enum { AUTO, USER } type;
    char* name;
    Label next;
};

// Var list structure, is member of Scope
typedef struct var_t* Var;
struct var_t{
    char* name;
    // type?
    Var next;
};

// represents the concept of code blocks
// also used for variables' accessibility check
typedef struct scope_t* Scope;
struct scope_t{
    int id;
    Var varList;
    Label autoLabelList;
    Scope father;
    Scope right;
    Scope child;
};

// function list
typedef struct function_t* Function;
struct function_t{
    char* name;
    int numOfParams;
    Function next;
};

string TreeNodeName[14];

// RET and NIT are self-defined node types
// RET: 2 children, move return value to TEMP(ret);
// NIL: empty node
enum { 
    CONST, NAME, TEMP, BINOP, 
    MEM, CALL, ESEQ, MOVE, 
    EXPR, JUMP, CJUMP, SEQ, LABEL,
    NIL
};

extern string OpName[24];

typedef enum { C_INT, C_FLOAT, C_STRING, C_BOOLEAN } ConstType;

typedef struct const_value_t* Const;
struct const_value_t{
    ConstType type;
    union {
        long long intV;
        double floatV;
        string stringV;
        boolean booleanV;
    } v;
};

// Entrance of IR tree generation module
TreeNode IRTree(pNode root);
// Redirection function
TreeNode IRHerald(pNode node);

TreeNode IRBlock(pNode node);
TreeNode IRStmts(pNode node);
TreeNode IRVarDec(pNode node);
TreeNode IRVarInit(pNode node);
TreeNode IRId(pNode node);
TreeNode IRFunction(pNode node);
TreeNode IRName(pNode node);
TreeNode IRLoadParams(pNode node);
TreeNode IRNil();
TreeNode IRMoveT(pNode node, char* tempName);
TreeNode IRTemp(char* name);
TreeNode IRLeafName(char* name);
TreeNode IRIf(pNode node);
TreeNode IRAutoLabel(char* name, char** newName);
TreeNode IRCjump(char* op, pNode expression, char* ToF, char* Tlabel, char* Flabel);
TreeNode IRSeq(TreeNode left, TreeNode right);
TreeNode IRJump(char* name);
TreeNode IRSwitch(pNode node);
TreeNode IRFor(pNode node);
TreeNode IRWhile(pNode node);
TreeNode IRDo(pNode node);
TreeNode IRLabel(pNode node);
TreeNode IRReturn(pNode node);
TreeNode IRBreak(pNode node);
TreeNode IRContinue(pNode node);
TreeNode IRGoto(pNode node);
TreeNode IREmptyStmt(pNode node);
TreeNode IRExps(pNode node);
TreeNode IRAssign(pNode node);
TreeNode IRTrinary(pNode node);
TreeNode IRBinary(pNode node);
TreeNode IROp(Op operatorr);
TreeNode IRPreUnary(pNode node);
TreeNode IRConst(Const c);
TreeNode IRPostUnary(pNode node);
TreeNode IRCall(pNode node);
TreeNode IRStoreParams(pNode node);
TreeNode IRLoadT(pNode node, char* tempName);
TreeNode IRFactor(pNode node);



// utilities
void init();

Const newConst();

void newLabel(char* name);
char* newAutoLabel(char* name);

void newVar(char* name);

void newFunction(pNode node);

Scope newScope();

TreeNode newTreeNode();
TreeNode* newNodeList(int size);

char* appendName(char* head, int num);

char* int2string(int num);
char* float2string(double f);
char* bool2string(boolean b);

// not implemented
void error();
void errorCheck();

#endif // !IR_TREE_H__


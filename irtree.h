#ifndef IR_TREE_H__
#define IR_TREE_H__

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;

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

string TreeNodeName[] = {
    "CONST", "NAME", "TEMP", "BINOP",
    "MEM", "CALL", "ESEQ", "MOVE", 
    "EXP", "JUMP", "CJUMP", "SEQ", "LABEL",
    "NIL"
};

enum { 
    CONST, NAME, TEMP, BINOP, 
    MEM, CALL, ESEQ, MOVE, 
    EXP, JUMP, CJUMP, SEQ, LABEL,
    NIL
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
TreeNode IRParams(pNode node);
TreeNode IRNil();
TreeNode IRMoveT(pNode node, char* tempName);
TreeNode IRTemp(char* name);
TreeNode IRLeafName(char* name);
TreeNode IRIf(pNode node);
TreeNode IRAutoLabel(char* name);
TreeNode IRCjump(char* op, pNode expression, char* ToF, char* Tlabel, char* Flabel);
TreeNode IRSeq(TreeNode left, TreeNode right);
TreeNode IRJump(char* name);
TreeNode IRSwitch(pNode node);
TreeNode IRFor(pNode node);
TreeNode IRWhile(pNode node);
TreeNode IRDo(pNode node);
TreeNode IRLabel(pNode node);


// utilities
void init();

void newLabel(char* name);
void newAutoLabel(char* name);

void newVar(char* name);

void newFunction(pNode node);

Scope newScope();

TreeNode newTreeNode();
TreeNode* newNodeList(int size);

void appendName(char* head, int num);

#endif // !IR_TREE_H__


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

// Var list structure, is member of Scope
typedef struct var_t* Var;
struct var_t{
    char* name;
    // type?
    Var next;
}

// represents the concept of code blocks
// also used for variables' accessibility check
typedef struct scope_t* Scope;
struct scope_t{
    int id;
    Var varList;
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
    "EXP", "JUMP", "SEQ", "LABEL",
    "NIL"
};

enum { 
    CONST, NAME, TEMP, BINOP, 
    MEM, CALL, ESEQ, MOVE, 
    EXP, JUMP, SEQ, LABEL,
    NIL
};

// Entrance of IR tree generation module
TreeNode IRTree(pNode root);




#endif // !IR_TREE_H__


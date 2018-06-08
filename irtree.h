#ifndef IR_TREE_H__
#define IR_TREE_H__

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;
typedef struct ir_node_* TreeNode;
struct ir_node_{
    string name;
    TreeNode* childs;
};

string TreeNodeName[] = {
    "CONST", "NAME", "TEMP", "BINOP",
    "MEM", "CALL", "ESEQ", "MOVE", 
    "EXP", "JUMP", "SEQ", "LABEL"
};

enum { 
    CONST, NAME, TEMP, BINOP, 
    MEM, CALL, ESEQ, MOVE, 
    EXP, JUMP, SEQ, LABEL
};

typedef enum { ENTRANCE, GOTO, ESCAPE } IRLabelType;

// Entrance of IR tree generation module
TreeNode IRTree(pNode root);

TreeNode IRLabel()


#endif // !IR_TREE_H__


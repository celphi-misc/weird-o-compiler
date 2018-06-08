#include "irtree.h"

// IR tree structure
TreeNode IRtree;
// total number of scopes
int numOfScope;
// using a skip list to maintain the scope hiarachy 
Scope scopeTree;
Scope currentScope;
// using a link list to maintain all functions
Function functionList;
Function currentFunction;

int sizeOfTreeNode = sizeof(struct ir_node_t);

TreeNode IRTree(pNode root){
    // initial data structures
    init();
    // start iteration
    IRtree = IRHerald(root);

    // 2 pass: error check
    errorCheck();
    return IRtree;
}

TreeNode IRBlock(pNode node){
    return IRHerald(node->u.blockExp.stmts);
}

TreeNode IRStmts(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[ESEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRHerald(node->u.stmtsExp.left);
    (this->childs)[1] = IRHerald(node->u.stmtsExp.right);
    return this;
}

TreeNode IRVarDec(pNode node){
    // Actually var declare do not have any presentation in tree 
    // maybe change to registering variables (in a varList?)
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[NAME];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRHerald(node->u.varDecExp.id);
    return this;
}

TreeNode IRFunction(pNode node){
    currentScope = newScope();
    // register function
    newFunction(node->u.functionExp.id);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRLabel(node->u.functionExp.id);

    (this->childs)[1] = newTreeNode();
    (this->childs)[1]->name = TreeNodeName[SEQ];
    (this->childs)[1]->pos = node->pos;
    (this->childs)[1]->numOfChild = 2;
    (this->childs)[1]->childs = newNodeList(2);

    ((this->childs)[1]->childs)[0] = IRParams(node->u.functionExp.para_list);
    ((this->childs)[1]->childs)[1] = IRBlock(node->u.functionExp.block);

    currentScope = currentScope->father;
    return this;
}

TreeNode IRParams(pNode node){

    if(node->kind == A_VOID){
        currentFunction->numOfParams = 0;
        TreeNode this = newTreeNode();
        this->name = TreeNodeName[NIL];
        this->pos = node->pos;
        this->numOfChild = 0;
        this->childs = NULL;
        return this;
    } else {
        // 1. register function's # of params,
        // 2. register vars in contemporary scope's varList
        // 3. move params from temps
        int count = 1;
        if(node->kind == A_ID){
            currentFunction->numOfParams = count;
            
            return IRMove(node->u.name);
        }
        pNode p = node;
        while(p->kind == A_EXPS){
            TreeNode this = newTreeNode();
            this->name = TreeNodeName[SEQ];
            this->pos = p->pos;
            this->numOfChild = 2;
            this->childs = newNodeList(2);
            (this->childs)[0] = IRMove(p->u.exps.left);
            count ++;
            p = p->u.exps.right;
        }
        currentFunction->numOfParams = count;

    }

}

TreeNode IRId(pNode node){

}

TreeNode IRHerald(pNode node){
    switch(node->kind){
        case A_BLOCK:
            return IRBlock(node);
        case A_STMTS:
            return IRStmts(node);
        case A_VAR_DEC: 
            return IRVarDec(node);
        case A_FUNCTION:
            return IRFunction(node);
        case A_ID:
            return IRId(node);
    }
}

void newFunction(pNode node){
    Function newF = (Function)malloc(sizeof(*newF));
    strcpy(newF->name, node->u.name);
    newF->next = functionList;
    functionList = newF;
    currentFunction = functionList;
}

Scope newScope(){
    numOfScope++;
    Scope newS = (Scope)malloc(sizeof(*newS));
    newS->varList = NULL;
    newS->father = currentScope;
    newS->right = NULL;
    newS->child = NULL;
    Scope p = currentScope->child;
    if(!p) {
        currentScope->child = newS;
    } else {
        while(p->right) p = p->right;
        p->right = newS;
    }
    return p;
}

void init(){
    // IRtree = (TreeNode)malloc(sizeof(*IRTree));
    numOfScope = 0;
    scopeTree = (Scope)malloc(sizeof(*scopeTree));
    scopeTree->id = numOfScope;
    scopeTree->father = NULL;
    scopeTree->right = NULL;
    scopeTree->child = NULL;
    currentScope = scopeTree;

    functionList = NULL;
    currentFunction = functionList;
}

// malloc space for a tree node struct
TreeNode newTreeNode(){
    return (TreeNode)malloc(sizeOfTreeNode);
}
// malloc space for a array of tree node pointers
TreeNode* newNodeList(int num){
    return (TreeNode*)malloc(num*sizeof(TreeNode));
}
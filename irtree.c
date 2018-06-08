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
    (this->childs)[0] = IRName(node->u.functionExp.id);

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

TreeNode IRName(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[NAME];
    this->pos = node->pos;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(node->u.name);
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
        // 3. move params from memory <<<<< OR TEMP? How to Implement?
        int count = 1;
        if(node->kind == A_ID){
            currentFunction->numOfParams = count;
            newVar(node->u.name);
            char* tempName = registerName("s", count-1);
            // MOVE: Para1 <- S1
            return IRMoveT(node, tempName);
        }
        pNode p = node;
        TreeNode this = newTreeNode();
        TreeNode ptr = this;
        while(p->kind == A_EXPS){
            ptr->name = TreeNodeName[SEQ];
            ptr->pos = p->pos;
            ptr->numOfChild = 2;
            ptr->childs = newNodeList(2);
            char* tempName = registerName("s", count-1);
            (ptr->childs)[0] = IRMoveT(p->u.exps.left, tempName);

            count++;
            p = p->u.exps.right;
            // last node in para_list is A_ID in kind
            if(p->kind == A_EXPS){
                (ptr->childs)[1] = newTreeNode();
                ptr = (ptr->childs)[1];
            }
        }
        currentFunction->numOfParams = count;
        char* tempName = registerName("s", count-1);
        (ptr->childs)[1] = IRMoveT(p, tempName);        
        return this;
    }
}

TreeNode IRMoveT(pNode node, char* tempName){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRLeafName(node->u.name);
    (this->childs)[1] = IRTemp(tempName);
    return this;
}

TreeNode IRTemp(char* tempName){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[TEMP];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(tempName);
    return this;
}

TreeNode IRId(pNode node){

}

TreeNode IRLeafName(char* name){
    TreeNode this = newTreeNode();
    this->name = (char*)malloc(strlen(name)+1);
    strcpy(this->name, name);
    this->pos = -1;
    this->numOfChild = 0;
    this->childs = NULL;
    return this;
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

    }
}

void newVar(char* name){
    Var newV = (Var)malloc(sizeof(*newV));
    newV->name = (char*)malloc(strlen(name)+1);
    strcpy(newV->name, name);
    newV->next = currentScope->varList;
    currentScope->varList = newV;
}

void newFunction(pNode node){
    Function newF = (Function)malloc(sizeof(*newF));
    newF->name = (char*)malloc(strlen(node->u.name)+1);
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

char* registerName(char* head, int num){
    int digit = 0;
    if(num == 0){
        digit = 1;
    }else{
        int copy = num;
        while(copy>0) {
            digit++;
            copy/=10;
        }
    }
    char* itoa = (char*)malloc(digit+1);
    int copy = num;
    for(int i = digit -1; i >= 0; i--){
        itoa[i] = copy %10;
        copy /=10;
    }
    itoa[digit] = '\0';
    int retLength = strlen(head) + digit;
    char* ret = (char*)malloc(retLength + 1);
    for(int i = 0; i<strlen(head); i++){
        ret[i] = head[i];
    }
    for(int i = strlen(head); i<retLength; i++){
        ret[i] = itoa[i - strlen(head)];
    }
    ret[retLength] = '\0';
    return ret;
}
#include "irtree.h"

int error = 0;
// IR tree structure
TreeNode IRtree;
// total number of scopes
Label globalLabelList;

string OpName[24] = {
    "PLUS", "MINUS", "INC", "DEC", "NOT", "BNOT",
    "TIMES", "DIV", "MOD",
    "BLEFT", "BRIGHT",
    "LE", "GE", "LT", "GT",
    "EQ", "NEQ",
    "BAND", "BXOR", "BOR",
    "AND", "OR", "DOT", "BRACKET"
};

string TreeNodeName[14] = {
    "CONST", "NAME", "TEMP", "BINOP",
    "MEM", "CALL", "ESEQ", "MOVE", 
    "EXP", "JUMP", "CJUMP", "SEQ", "LABEL",
    "NIL"
};

int numOfScope;
// using a skip list to maintain the scope hiarachy 
Scope scopeTree;
Scope currentScope;
// using a link list to maintain all functions
Function functionList;
Function currentFunction;

int sizeOfTreeNode = sizeof(struct ir_node_t);

TreeNode IRTree(pNode root){
#ifdef _DEBUG
    fprintf(stdout, "in IRTree\n");
#endif
    // initial data structures
    init();
    // start iteration
    IRtree = IRHerald(root);

    // 2 pass: error check
    errorCheck();
    return IRtree;
}

TreeNode IRBlock(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRBlock\n");
#endif
    return IRHerald(node->u.blockExp.stmts);
}

TreeNode IRStmts(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRStmts\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[ESEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRHerald(node->u.stmtsExp.left);
    (this->childs)[1] = IRHerald(node->u.stmtsExp.right);
    if(!error) return this; else return NULL;
}

TreeNode IRVarDec(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRVarDec\n");
#endif
    // Actually var declare do not have any presentation in tree 
    // maybe change to registering variables (in a varList?)
    char* varName = node->u.varDecExp.id->u.name;
    newVar(varName);
    return IRId(node->u.varDecExp.id);
}

TreeNode IRVarInit(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRVarInit\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    char* varName = node->u.varInitExp.id->u.name;
    newVar(varName);
    (this->childs)[0] = IRId(node->u.varInitExp.id);
    (this->childs)[1] = IRHerald(node->u.varInitExp.init);
    if(!error) return this; else return NULL;
}

TreeNode IRId(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRId\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[TEMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(node->u.name);
    if(!error) return this; else return NULL;
}

TreeNode IRFunction(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRFunction\n");
#endif
    currentScope = newScope();
    // register function
    newFunction(node->u.functionExp.id);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[ESEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
// left child: (NAME, SEQ(PARAMS, BLOCK))
    (this->childs)[0] = newTreeNode();
    TreeNode left = (this->childs)[0];
    left->name = TreeNodeName[SEQ];
    left->pos = node->pos;
    left->numOfChild = 2;
    left->childs = newNodeList(2);
    (left->childs)[0] = IRName(node->u.functionExp.id);
    (left->childs)[1] = newTreeNode();
    (left->childs)[1]->name = TreeNodeName[SEQ];
    (left->childs)[1]->pos = node->pos;
    (left->childs)[1]->numOfChild = 2;
    (left->childs)[1]->childs = newNodeList(2);
    ((left->childs)[1]->childs)[0] = IRLoadParams(node->u.functionExp.para_list);
    ((left->childs)[1]->childs)[1] = IRBlock(node->u.functionExp.block);
// right child: (LABEL(ret), TEMP(ret));
    char* nameRet = "ret";
    char** newName = (char**)malloc(sizeof(char*));
    TreeNode labelRet = IRAutoLabel(nameRet, newName);
    nameRet = *newName;

    (this->childs)[1] = newTreeNode();
    TreeNode right = (this->childs)[1];
    right->name = TreeNodeName[ESEQ];
    right->pos = node->pos;
    right->numOfChild = 2;
    right->childs = newNodeList(2);
    (right->childs)[0] = labelRet;
    (right->childs)[1] = IRTemp("ret");

    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRName(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRName\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[NAME];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(node->u.name);
    if(!error) return this; else return NULL;
}

TreeNode IRLoadParams(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRLoadParams\n");
#endif

    if(node->kind == A_VOID){
        currentFunction->numOfParams = 0;
        return IRNil();
    } else {
        // 1. register function's # of params,
        // 2. register vars in contemporary scope's varList
        // 3. move params from memory <<<<< OR TEMP? How to Implement?
        int count = 1;
        if(node->kind != A_EXPS){
            currentFunction->numOfParams = count;
            newVar(node->u.name);
            char* tempName = "s";
            tempName = appendName(tempName, count-1);
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
            char* tempName = "s";
            tempName = appendName(tempName, count-1);
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
        char* tempName = "s";
        tempName = appendName(tempName, count-1);
        (ptr->childs)[1] = IRMoveT(p, tempName);        
        if(!error) return this; else return NULL;
    }
}

TreeNode IRNil(){
#ifdef _DEBUG
    fprintf(stdout, "in IRNil\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[NIL];
    this->pos = -1;
    this->numOfChild = 0;
    this->childs = NULL;
    if(!error) return this; else return NULL;
}

TreeNode IRMoveT(pNode node, char* tempName){
#ifdef _DEBUG
    fprintf(stdout, "in IRMoveT\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRLeafName(node->u.name);
    (this->childs)[1] = IRTemp(tempName);
    if(!error) return this; else return NULL;
}

TreeNode IRTemp(char* tempName){
#ifdef _DEBUG
    fprintf(stdout, "in IRTemp\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[TEMP];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(tempName);
    if(!error) return this; else return NULL;
}

TreeNode IRLeafName(char* name){
#ifdef _DEBUG
    fprintf(stdout, "in IRLeafName\n");
#endif
    TreeNode this = newTreeNode();
    this->name = (char*)malloc(strlen(name)+1);
    strcpy(this->name, name);
    this->pos = -1;
    this->numOfChild = 0;
    this->childs = NULL;
    if(!error) return this; else return NULL;
}

TreeNode IRIf(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRIf\n");
#endif
    currentScope = newScope();
// generate three auto labels
    char* nameT = "TRUE";
    char* nameF = "FALSE";
    char* nameD = "DONE";
    char** newName = (char**)malloc(sizeof(char*));
    TreeNode labelT = IRAutoLabel(nameT, newName);
    nameT = *newName;
    TreeNode labelF = IRAutoLabel(nameF, newName);
    nameF = *newName;
    TreeNode labelD = IRAutoLabel(nameD, newName);
    nameD = *newName;

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
// CJUMP: if(exp==TRUE) then Tlabel else Flabel
    Const c = newConst();
    c->type = C_BOOLEAN;
    c->v.booleanV = FALSE;
    (this->childs)[0] = IRCjump("EQ", node->u.ifExp.test, IRConst(c), nameT, nameF);
// SEQ Tree: first TRUE, then FALSE, then Done
    (this->childs)[1] = IRSeq(
        labelT,IRSeq(
            IRHerald(node->u.ifExp.if_clause), IRSeq(
                IRJump(nameD), IRSeq(
                    labelF, IRSeq(
                        IRHerald(node->u.ifExp.else_clause), labelD
                    )
                )
            )
        )
    );
    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRAutoLabel(char* name, char** newName){
#ifdef _DEBUG
    fprintf(stdout, "in IRAutoLabel\n");
#endif
    // newAutoLabel will append id to name, 
    // and save the new label in labelList;
    *newName = newAutoLabel(name);
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[LABEL];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(*newName);
    if(!error) return this; else return NULL;
}

TreeNode IRCjump(char* op, pNode expression, TreeNode cmp, char* Tlabel, char* Flabel){
#ifdef _DEBUG
    fprintf(stdout, "in IRCjump\n");
#endif
// CJUMP
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[CJUMP];
    this->pos = expression->pos;
    this->numOfChild = 5;
    this->childs = newNodeList(5);
    (this->childs)[0] = IRLeafName(op);
    (this->childs)[1] = IRHerald(expression);
    (this->childs)[2] = cmp;
    (this->childs)[3] = IRLeafName(Tlabel);
    (this->childs)[4] = IRLeafName(Flabel);
    if(!error) return this; else return NULL;
}

TreeNode IRSeq(TreeNode left, TreeNode right){
#ifdef _DEBUG
    fprintf(stdout, "in IRSeq\n");
#endif
// check if there is NULL EXP
    if(left && right){
        TreeNode this = newTreeNode();
        this->name = TreeNodeName[SEQ];
        this->pos = left->pos;
        this->numOfChild = 2;
        this->childs = newNodeList(2);
        (this->childs)[0] = left;
        (this->childs)[1] = right;
        if(!error) return this; else return NULL;
    } else if (left){
        return left;
    } else if (right){
        return right;
    } else {
        return IRNil();
    }
}

TreeNode IRJump(char* name){
#ifdef _DEBUG
    fprintf(stdout, "in IRJump\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(name);
    if(!error) return this; else return NULL;
}

TreeNode IRSwitch(pNode node){
// WO BU XIE LE!
// ZHEN XIANG!
    return IRNil();
}

TreeNode IRFor(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRFor\n");
#endif
    currentScope = newScope();

    char* nameTest = "test";
    char* nameStart = "start";
    char* nameCont = "cont";
    char* nameBreak = "break";
    char** newName = (char**)malloc(sizeof(char*));
    TreeNode labelTest = IRAutoLabel(nameTest, newName);
    nameTest = *newName;
    TreeNode labelStart = IRAutoLabel(nameStart, newName);
    nameStart = *newName;
    TreeNode labelCont = IRAutoLabel(nameCont, newName);
    nameCont = *newName;
    TreeNode labelBreak = IRAutoLabel(nameBreak, newName);
    nameBreak = *newName;

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRHerald(node->u.forExp.start);
    Const c= newConst();
    c->type = C_BOOLEAN;
    c->v.booleanV = TRUE;
    (this->childs)[1] = IRSeq(
        IRSeq(labelTest, IRCjump("EQ", node->u.forExp.test, IRConst(c), nameStart, nameBreak)), IRSeq(
            labelStart, IRSeq(
                IRHerald(node->u.forExp.body), IRSeq(
                    labelCont,IRSeq(
                        IRHerald(node->u.forExp.incr), IRSeq(
                            IRJump(nameTest), labelBreak)
                        )
                    )
                )
            )
        );
    
    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRWhile(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRWhile\n");
#endif
    currentScope = newScope();

    char* nameTest = "test";
    char* nameCont = "cont";
    char* nameBreak = "break";
    char** newName = (char**)malloc(sizeof(char*));
    TreeNode labelTest = IRAutoLabel(nameTest, newName);
    nameTest = *newName;
    TreeNode labelCont = IRAutoLabel(nameCont, newName);
    nameCont = *newName;
    TreeNode labelBreak = IRAutoLabel(nameBreak, newName);
    nameBreak = *newName;

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    Const c= newConst();
    c->type = C_BOOLEAN;
    c->v.booleanV = TRUE;
    (this->childs)[0] = IRSeq(
        labelTest, IRCjump("EQ", node->u.whileExp.test, IRConst(c), nameCont, nameBreak));
    (this->childs)[1] = IRSeq(
        labelCont, IRSeq(
            IRHerald(node->u.whileExp.body), IRSeq(
                IRJump(nameTest), labelBreak
            )
        )
    );

    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRDo(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRDo\n");
#endif
    currentScope = newScope();

    char* nameTest = "test";
    char* nameCont = "cont";
    char* nameBreak = "break";
    char** newName = (char**)malloc(sizeof(char*));
    TreeNode labelTest = IRAutoLabel(nameTest, newName);
    nameTest = *newName;
    TreeNode labelCont = IRAutoLabel(nameCont, newName);
    nameCont = *newName;
    TreeNode labelBreak = IRAutoLabel(nameBreak, newName);
    nameBreak = *newName;

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRSeq(
        labelCont, IRSeq(
            IRHerald(node->u.doExp.body), IRSeq(
                IRJump(nameTest), labelBreak
            )
        )
    );
    Const c= newConst();
    c->type = C_BOOLEAN;
    c->v.booleanV = TRUE;
    (this->childs)[1] = IRSeq(
        labelTest, IRCjump("EQ", node->u.doExp.test, IRConst(c), nameCont, nameBreak)
    );

    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRLabel(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRLabel\n");
#endif
    char* name = node->u.labelExp.id->u.name;
    newLabel(name);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[LABEL];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(name);
    if(!error) return this; else return NULL;
}

TreeNode IRReturn(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRReturn\n");
#endif
   TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
// left child: MOVE(ret, EXP(e))
    (this->childs)[0] = newTreeNode();
    TreeNode left = (this->childs)[0];
    left->name = TreeNodeName[MOVE];
    left->pos = node->pos;
    left->numOfChild = 2;
    left->childs = newNodeList(2);
    char* tempName = "ret";
    (left->childs)[0] = IRTemp(tempName);
    (left->childs)[1] = IRHerald(node->u.returnExp.exp);
// right child: JUMP(ret);
    (this->childs)[1] = newTreeNode();
    TreeNode right = (this->childs)[1];
    right->name = TreeNodeName[JUMP];
    right->pos = node->pos;
    right->numOfChild = 1;
    right->childs = newNodeList(1);
    char* jumpName = "ret";
    jumpName = appendName(jumpName, currentScope->id);
#ifdef _DEBUG
    fprintf(stdout, "here");
#endif
    (right->childs)[0] = IRLeafName(jumpName);
    if(!error) return this; else return NULL;
}

TreeNode IRBreak(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRBreak\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    char* name = "break";
    name = appendName(name, currentScope->id);
    (this->childs)[0] = IRLeafName(name);
    if(!error) return this; else return NULL;
}

TreeNode IRContinue(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRContinue\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    char* name = "cont";
    name = appendName(name, currentScope->id);
    (this->childs)[0] = IRLeafName(name);
    if(!error) return this; else return NULL;
}

TreeNode IRGoto(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRGoto\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    char* name = node->u.gotoExp.id->u.name;
    (this->childs)[0] = IRLeafName(name);
    if(!error) return this; else return NULL;
}

TreeNode IREmptyStmt(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IREmptyStmt\n");
#endif
    return IRNil();
}

TreeNode IRExps(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRExps\n");
#endif
    if(node->u.exps.right){
        TreeNode this = newTreeNode();
        this->name = TreeNodeName[ESEQ];
        this->pos = node->pos;
        this->numOfChild = 2;
        this->childs = newNodeList(2);
        (this->childs)[0] = IRHerald(node->u.exps.left);
        (this->childs)[1] = IRHerald(node->u.exps.right);
        if(!error) return this; else return NULL;
    } else {
        return IRHerald(node->u.exps.left);
    }
}

TreeNode IRAssign(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRAssign\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRHerald(node->u.assignExp.left);
    (this->childs)[1] = IRHerald(node->u.assignExp.right);
    if(!error) return this; else return NULL;
}

// same to if-else
TreeNode IRTrinary(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRTrinary\n");
#endif
    currentScope = newScope();
// generate three auto labels
    char* nameT = "TRUE";
    char* nameF = "FALSE";
    char* nameD = "DONE";
    char** newName = (char**)malloc(sizeof(char*));
    TreeNode labelT = IRAutoLabel(nameT, newName);
    nameT = *newName;
    TreeNode labelF = IRAutoLabel(nameF, newName);
    nameF = *newName;
    TreeNode labelD = IRAutoLabel(nameD, newName);
    nameD = *newName;

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[ESEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
// CJUMP: if(exp==TRUE) then Tlabel else Flabel    
    Const c= newConst();
    c->type = C_BOOLEAN;
    c->v.booleanV = TRUE;
    (this->childs)[0] = IRCjump("EQ", node->u.trinaryExp.test, IRConst(c), nameT, nameF);
// SEQ Tree: first TRUE, then FALSE, then Done
    (this->childs)[1] = IRSeq(
        labelT,IRSeq(
            IRHerald(node->u.trinaryExp.ift), IRSeq(
                IRJump(nameD), IRSeq(
                    labelF, IRSeq(
                        IRHerald(node->u.trinaryExp.iff), labelD
                    )
                )
            )
        )
    );
    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRBinary(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRBinary\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[BINOP];
    this->pos = node->pos;
    this->numOfChild = 3;
    this->childs = newNodeList(3);
    (this->childs)[0] = IROp(node->u.binaryExp.op);
    (this->childs)[1] = IRHerald(node->u.binaryExp.left);
    (this->childs)[2] = IRHerald(node->u.binaryExp.right);
    if(!error) return this; else return NULL;
}

TreeNode IROp(Op operatorr){
#ifdef _DEBUG
    fprintf(stdout, "in IROp\n");
#endif
    return IRLeafName(OpName[operatorr]);
}

TreeNode IRPreUnary(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRPreUnary\n");
#endif
// I just regard it as BINOP 
    if(node->u.preUnaryExp.op){
        TreeNode this = newTreeNode();
        this->name = TreeNodeName[BINOP];
        this->pos = node->pos;
        this->numOfChild = 3;
        this->childs = newNodeList(3);
        (this->childs)[0] = IROp(node->u.preUnaryExp.op);
        (this->childs)[1] = IRHerald(node->u.preUnaryExp.exp);
        Const c = newConst();
        c->type = C_INT;
        c->v.intV = 0;
        (this->childs)[2] = IRConst(c);
        if(!error) return this; else return NULL;
    } else {
        return IRHerald(node->u.preUnaryExp.exp);
    }
}

TreeNode IRConst(Const c){
#ifdef _DEBUG
    fprintf(stdout, "in IRConst\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[CONST];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    switch(c->type){
        case C_INT:
            this->isStr = 0;
            (this->childs)[0] = IRLeafName(int2string((int)c->v.intV));
            break;
        case C_FLOAT:
            this->isStr = 0;
            (this->childs)[0] = IRLeafName(float2string(c->v.floatV));
            break;
        case C_STRING:
            (this->childs)[0] = IRLeafName(c->v.stringV);
            break;
        case C_BOOLEAN:
            this->isStr = 0;
            (this->childs)[0] = IRLeafName(bool2string(c->v.booleanV));
            break; 
    }
    if(!error) return this; else return NULL;
}

TreeNode IRPostUnary(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRPostUnary\n");
#endif
    return IRPreUnary(node);
}

TreeNode IRCall(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRCall\n");
#endif
    currentScope = newScope();

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[ESEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRStoreParams(node->u.callExp.arg_list);
    (this->childs)[1] = newTreeNode();
    TreeNode right = (this->childs)[1];
    right->name = TreeNodeName[CALL];
    right->pos = node->pos;
    right->numOfChild = 1;
    right->childs = newNodeList(1);
    char* callName = node->u.callExp.head->u.name;
    (right->childs)[0] = IRLeafName(callName);
    currentScope = currentScope->father;
    if(!error) return this; else return NULL;
}

TreeNode IRStoreParams(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRStoreParams\n");
#endif
    if(node->kind == A_VOID){
        return IRNil();
    } else {
        // move params to TEMP
        int count = 1;
        if(node->kind != A_EXPS){
            char* tempName = "s";
            appendName(tempName, count-1);
            // MOVE: Para1 <- S1
            return IRLoadT(node, tempName);
        }
        pNode p = node;
        TreeNode this = newTreeNode();
        TreeNode ptr = this;
        while(p->kind == A_EXPS){
            ptr->name = TreeNodeName[SEQ];
            ptr->pos = p->pos;
            ptr->numOfChild = 2;
            ptr->childs = newNodeList(2);
            char* tempName = "s";
            tempName = appendName(tempName, count-1);
            (ptr->childs)[0] = IRLoadT(p->u.exps.left, tempName);
            count++;
            p = p->u.exps.right;
            // last node in para_list is A_ID in kind
            if(p->kind == A_EXPS){
                (ptr->childs)[1] = newTreeNode();
                ptr = (ptr->childs)[1];
            }
        }
        char* tempName = "s";
        tempName = appendName(tempName, count-1);
        (ptr->childs)[1] = IRLoadT(p, tempName); 
        if(!error) return this; else return NULL;
    }
}

TreeNode IRLoadT(pNode node, char* tempName){
#ifdef _DEBUG
    fprintf(stdout, "in IRLoadT\n");
#endif
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRTemp(tempName);
    (this->childs)[1] = IRHerald(node);
    if(!error) return this; else return NULL;
}

TreeNode IRFactor(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRFactor\n");
#endif
    Const c = newConst();
    switch(node->kind){
        case A_INT:
            c->type = C_INT;
            c->v.intV = node->u.intVal;
            break;
        case A_FLOAT:
            c->type = C_FLOAT;
            c->v.floatV = node->u.floatVal;
            break;
        case A_STRING:
            c->type = C_STRING;
            c->v.stringV = node->u.stringVal;
            break;
        case A_BOOLEAN:
            c->type = C_BOOLEAN;
            c->v.booleanV = node->u.booleanVal;
            break;
        case A_NUL:
            return IRNil();
        default:
            printError();
    }
    return IRConst(c);
}

TreeNode IRHerald(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in IRHerald\n");
#endif
    switch(node->kind){
        case A_VOID:
            return IRNil();
        case A_BLOCK:
            return IRBlock(node);
        case A_STMTS:
            return IRStmts(node);
        case A_VAR_DEC: 
            return IRVarDec(node);
        case A_VAR_INIT:
            return IRVarInit(node);
        case A_FUNCTION:
            return IRFunction(node);
        case A_IF:
            return IRIf(node);
        case A_SWITCH:
        case A_CASE:
        case A_DEFAULT:
            return IRSwitch(node);
        case A_FOR:
            return IRFor(node);
        case A_WHILE:
            return IRWhile(node);
        case A_DO:
            return IRDo(node);
        case A_LABEL:
            return IRLabel(node);
        case A_RETURN:
            return IRReturn(node);
        case A_BREAK:
            return IRBreak(node);
        case A_CONTINUE:
            return IRContinue(node);
        case A_GOTO:
            return IRGoto(node);
        case A_EMPTY_STMT:
            return IREmptyStmt(node);
        case A_EXPS:
            return IRExps(node);
        case A_ASSIGN:
            return IRAssign(node);
        case A_TRINARY:
            return IRTrinary(node);
        case A_BINARY:
            return IRBinary(node);
        case A_PREUNARY:
            return IRPreUnary(node);
        case A_POSTUNARY:
            return IRPostUnary(node);
        case A_CALL:
            return IRCall(node);
        case A_INT:
        case A_FLOAT:
        case A_STRING:
        case A_BOOLEAN:
            return IRFactor(node);
        case A_NUL:
            return IRNil();
        case A_ID:
            return IRId(node);
        default:
            printError();
    }
    return IRNil();
}

Const newConst(){
#ifdef _DEBUG
    fprintf(stdout, "in newConst\n");
#endif
    return (Const)malloc(sizeof(struct const_value_t));
}

void newLabel(char* name){
#ifdef _DEBUG
    fprintf(stdout, "in newLabel\n");
#endif
    Label newL = (Label)malloc(sizeof(*newL));
    newL->name = (char*)malloc(strlen(name)+1);
    strcpy(newL->name, name);
    newL->type = USER;
    newL->next = globalLabelList;
    globalLabelList = newL;
}

char* newAutoLabel(char* name){
#ifdef _DEBUG
    fprintf(stdout, "in newAutoLabel\n");
#endif
    char* ret = appendName(name, currentScope->id);
    Label newL = (Label)malloc(sizeof(*newL));
    newL->name = (char*)malloc(strlen(name)+1);
    strcpy(newL->name, ret);
    newL->type = AUTO;
    newL->next = currentScope->autoLabelList;
    currentScope->autoLabelList = newL;
    return ret;
}

void newVar(char* name){
#ifdef _DEBUG
    fprintf(stdout, "in newVar\n");
#endif
#ifdef _DEBUG
    fprintf(stdout, "copying string\n");
#endif
    Var newV = (Var)malloc(sizeof(*newV));
    newV->name = (char*)malloc(strlen(name)+1);
    strcpy(newV->name, name);
#ifdef _DEBUG
    fprintf(stdout, "finish copy string\n");
#endif
    newV->next = currentScope->varList;
    currentScope->varList = newV;
}

void newFunction(pNode node){
#ifdef _DEBUG
    fprintf(stdout, "in newFunction\n");
#endif
    Function newF = (Function)malloc(sizeof(*newF));
    newF->name = (char*)malloc(strlen(node->u.name)+1);
    strcpy(newF->name, node->u.name);
    newF->next = functionList;
    functionList = newF;
    currentFunction = functionList;
}

Scope newScope(){
#ifdef _DEBUG
    fprintf(stdout, "in newScope\n");
#endif
    numOfScope++;
    Scope newS = (Scope)malloc(sizeof(*newS));
    newS->id = numOfScope;
    newS->varList = NULL;
    newS->autoLabelList = NULL;
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
    return newS;
}

void init(){
#ifdef _DEBUG
    fprintf(stdout, "in init\n");
#endif
    // IRtree = (TreeNode)malloc(sizeof(*IRTree));
    globalLabelList = NULL;

    numOfScope = 0;
    scopeTree = (Scope)malloc(sizeof(*scopeTree));
    scopeTree->id = numOfScope;
    scopeTree->varList = NULL;
    scopeTree->autoLabelList = NULL;
    scopeTree->father = NULL;
    scopeTree->right = NULL;
    scopeTree->child = NULL;
    currentScope = scopeTree;

    functionList = NULL;
    currentFunction = functionList;
}

// malloc space for a tree node struct
TreeNode newTreeNode(){
#ifdef _DEBUG
    fprintf(stdout, "in newTreeNode\n");
#endif
    TreeNode this = (TreeNode)malloc(sizeOfTreeNode);
    this->isStr = 1;
    return this;
}
// malloc space for a array of tree node pointers
TreeNode* newNodeList(int num){
    return (TreeNode*)malloc(num*sizeof(TreeNode));
}

char* appendName(char* head, int num){
#ifdef _DEBUG
    fprintf(stdout, "in appendName\n");
#endif
    char* itoa = int2string(num);
    int lengthDst = strlen(head);
    int lengthSrc = strlen(itoa);
    char* ret = malloc(sizeof(char)*(lengthDst+lengthSrc+1));
    for(int i = 0; i<lengthDst; i++){
        ret[i] = head[i];
    }
    for(int i = lengthDst; i< lengthDst + lengthSrc; i++){
        ret[i] = itoa[i - lengthDst];
    }
    ret[lengthDst + lengthSrc] = '\0';
    return ret;
}

char* int2string(int num){
#ifdef _DEBUG
    fprintf(stdout, "in int2string\n");
    fprintf(stdout, "int :          %d\n", num);
#endif
    int digit = 0;
    int copy = num;
    if(copy == 0) {
        char* ret = "0";
        return ret;
    }else {
        while(copy > 0){
            copy /= 10;
            digit++;
        } 
    }
    char* ret = (char*)malloc(sizeof(char)*(digit+1));
    copy = num;
    for(int i = digit-1; i>=0; i--){
        ret[i] = (char)(48+copy %10);
        copy /=10;
    }
    ret[digit] = '\0';
    return ret;
}

char* float2string(double f){
#ifdef _DEBUG
    fprintf(stdout, "in float2string\n");
#endif
    char* ret = (char*)malloc(sizeof(char) * 256);
    for(int i = 0; i<256; i++){
        ret[i] = 0;
    }
    sprintf(ret, "%lf", f);
    return ret;
}

char* bool2string(boolean b){
#ifdef _DEBUG
    fprintf(stdout, "in bool2string\n");
#endif
    if(b == TRUE){
        return "true";
    } else {
        return "false";
    }
}

void errorCheck(){

}
void printError(){

}

#define BUF_SIZE 0xfff2
char buffer[BUF_SIZE];

int printJson(TreeNode node, char* buf){
    if(!node) return 0;
    char* oldBuf = buf;
    if(node->numOfChild == 0){
        if(node->isStr)
            buf+=sprintf(buf, "{\"node\": \"%s\"", node->name);
        else
            buf+=sprintf(buf, "{\"node\": %s", node->name);
    } else if(node->numOfChild > 0){
        buf+=sprintf(buf, "{\"node\":\"%s\"", node->name);
        for(int i = 0; i< node->numOfChild; i++){
            buf+= sprintf(buf, ",\"%d\":", i);
            buf += printJson((node->childs)[i], buf);
        }
    }
    buf+=sprintf(buf, "}");
    return buf - oldBuf;
}

char* createIRJsonStr(TreeNode root){
    for(int i = 0; i< BUF_SIZE; i++){
        buffer[i] = 0;
    }
    printJson(root, buffer);
    char* ret = (char*)malloc(strlen(buffer)+1);
    strcpy(ret, buffer);
    return ret;
}

char bufferS[BUF_SIZE];

int printScopeContent(Scope node, char* buf){
    if(!node) return 0;
    char* oldBuf = buf;
    buf+=sprintf(buf, "{\"node\": \"scope\",");
    buf+=sprintf(buf, "\"id\": %d", node->id);
    if(node->varList){
        buf+=sprintf(buf, ",\"vars\": {");
        buf+=sprintf(buf, "\"node\": \"vars\"");
        Var p = node->varList;
        int i = 0;
        while(p){
            buf+=sprintf(buf, ",\"%d\": \"%s\"", i, p->name);
            p = p->next;
            i++;
        }
        buf+= sprintf(buf, "}");
    }
    if(node->child){
        buf+=sprintf(buf, ",\"childScope\": {");
        buf+= sprintf(buf, "\"node\": \"childScope\"");
        Scope child = node->child;
        int i = 0;
        while (child){
            buf+= sprintf(buf, ",\"%d\": ", i);
            buf+= printScopeContent(child, buf);
            child = child->right;
            i++;
        }
        buf+=sprintf(buf, "}");
    }
    buf+= sprintf(buf, "}");
    return buf - oldBuf;
}

char* printScopeAndVar(){
    for(int i = 0; i< BUF_SIZE; i++){
        bufferS[i] = 0;
    }
    printScopeContent(scopeTree, bufferS);
    char* ret = (char*)malloc(strlen(bufferS)+1);
    strcpy(ret, bufferS);
    return ret;
}
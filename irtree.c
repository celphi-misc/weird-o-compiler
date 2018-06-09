#include "irtree.h"

// IR tree structure
TreeNode IRtree;
// total number of scopes
Label globalLabelList;

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
    char* varName = node->u.varDecExp.id->u.name;
    newVar(varName);
    return IRId(node->u.varDecExp.id);
}

TreeNode IRVarInit(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    char* varName = node->u.varInitExp.id->u.name;
    newVar(varName);
    (this->childs)[0] = IRId(node->u.varInitExp.id);
    (this->childs)[1] = IRHerald(node->u.varInitExp.init);
    return this;
}

TreeNode IRId(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[TEMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(node->u.name);
    return this;
}

TreeNode IRFunction(pNode node){
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
    TreeNode labelRet = IRAutoLabel(nameRet);

    (this->childs)[1] = newTreeNode();
    TreeNode right = (this->childs)[1];
    right->name = TreeNodeName[ESEQ];
    right->pos = node->pos;
    right->numOfChild = 2;
    right->childs = newNodeList(2);
    (right->childs)[0] = labelRet;
    (right->childs)[1] = IRTemp("ret");

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

TreeNode IRLoadParams(pNode node){

    if(node->kind == A_VOID){
        currentFunction->numOfParams = 0;
        return IRNil();
    } else {
        // 1. register function's # of params,
        // 2. register vars in contemporary scope's varList
        // 3. move params from memory <<<<< OR TEMP? How to Implement?
        int count = 1;
        if(node->kind == A_ID){
            currentFunction->numOfParams = count;
            newVar(node->u.name);
            char* tempName = "s";
            appendName(tempName, count-1);
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
            appendName(tempName, count-1);
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
        appendName(tempName, count-1);
        (ptr->childs)[1] = IRMoveT(p, tempName);        
        return this;
    }
}

TreeNode IRNil(){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[NIL];
    this->pos = -1;
    this->numOfChild = 0;
    this->childs = NULL;
    return this;
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

TreeNode IRLeafName(char* name){
    TreeNode this = newTreeNode();
    this->name = (char*)malloc(strlen(name)+1);
    strcpy(this->name, name);
    this->pos = -1;
    this->numOfChild = 0;
    this->childs = NULL;
    return this;
}

TreeNode IRIf(pNode node){
    currentScope = newScope();
// generate three auto labels
    char* nameT = "TRUE";
    char* nameF = "FALSE";
    char* nameD = "DONE";
    TreeNode labelT = IRAutoLabel(nameT);
    TreeNode labelF = IRAutoLabel(nameF);
    TreeNode labelD = IRAutoLabel(nameD);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
// CJUMP: if(exp==TRUE) then Tlabel else Flabel
    (this->childs)[0] = IRCjump("EQ", node->u.ifExp.test, "TRUE", nameT, nameF);
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
    return this;
}

TreeNode IRAutoLabel(char* name){
    // newAutoLabel will append id to name, 
    // and save the new label in labelList;
    newAutoLabel(name);
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[LABEL];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(name);
    return this;
}

TreeNode IRCjump(char* op, pNode expression, char* ToF, char* Tlabel, char* Flabel){
// CJUMP
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[CJUMP];
    this->pos = expression->pos;
    this->numOfChild = 5;
    this->childs = newNodeList(5);
    (this->childs)[0] = IRLeafName(op);
    (this->childs)[1] = IRHerald(expression);
    (this->childs)[2] = IRLeafName(ToF);
    (this->childs)[3] = IRLeafName(Tlabel);
    (this->childs)[4] = IRLeafName(Flabel);
    return this;
}

TreeNode IRSeq(TreeNode left, TreeNode right){
// check if there is NULL EXP
    if(left && right){
        TreeNode this = newTreeNode();
        (this->childs)[1] = this;
        this->name = TreeNodeName[SEQ];
        this->pos = left->pos;
        this->numOfChild = 2;
        this->childs = newNodeList(2);
        (this->childs)[0] = left;
        (this->childs)[1] = right;
        return this;
    } else if (left){
        return left;
    } else if (right){
        return right;
    } else {
        return IRNil();
    }
}

TreeNode IRJump(char* name){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(name);
    return this;
}

TreeNode IRSwitch(pNode node){
// WO BU XIE LE!
// ZHEN XIANG!
    return IRNil();
}

TreeNode IRFor(pNode node){
    currentScope = newScope();

    char* nameTest = "test";
    char* nameStart = "start";
    char* nameCont = "cont";
    char* nameBreak = "break";
    TreeNode labelTest = IRAutoLabel(nameTest),
            labelStart = IRAutoLabel(nameStart),
            labelCont = IRAutoLabel(nameCont),
            labelBreak = IRAutoLabel(nameBreak);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRHerald(node->u.forExp.start);
    (this->childs)[1] = IRSeq(
        IRSeq(labelTest, IRCjump("EQ", node->u.forExp.test, "TRUE", nameStart, nameBreak)), IRSeq(
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
    return this;
}

TreeNode IRWhile(pNode node){
    currentScope = newScope();

    char* nameTest = "test";
    char* nameCont = "cont";
    char* nameBreak = "break";
    TreeNode labelTest = IRAutoLabel(nameTest),
            labelCont = IRAutoLabel(nameCont),
            labelBreak = IRAutoLabel(nameBreak);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[SEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRSeq(
        labelTest, IRCjump("EQ", node->u.whileExp.test, "TRUE", nameCont, nameBreak));
    (this->childs)[1] = IRSeq(
        labelCont, IRSeq(
            IRHerald(node->u.whileExp.body), IRSeq(
                IRJump(nameTest), labelBreak
            )
        )
    );

    currentScope = currentScope->father;
    return this;
}

TreeNode IRDo(pNode node){
    currentScope = newScope();

    char* nameTest = "test";
    char* nameCont = "cont";
    char* nameBreak = "break";
    TreeNode labelTest = IRAutoLabel(nameTest),
            labelCont = IRAutoLabel(nameCont),
            labelBreak = IRAutoLabel(nameBreak);

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
    (this->childs)[1] = IRSeq(
        labelTest, IRCjump("EQ", node->u.doExp.test, "TRUE", nameCont, nameBreak)
    );

    currentScope = currentScope->father;
    return this;
}

TreeNode IRLabel(pNode node){
    char* name = node->u.labelExp.id->u.name;
    newLabel(name);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[LABEL];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    (this->childs)[0] = IRLeafName(name);
    return this;
}

TreeNode IRReturn(pNode node){
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
    appendName(jumpName, currentScope->id);
    (right->childs)[0] = IRLeafName(jumpName);
    return this;
}

TreeNode IRBreak(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    char* name = "break";
    appendName(name, currentScope->id);
    (this->childs)[0] = IRLeafName(name);
    return this;
}

TreeNode IRContinue(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    char* name = "cont";
    appendName(name, currentScope->id);
    (this->childs)[0] = IRLeafName(name);
    return this;
}

TreeNode IRGoto(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[JUMP];
    this->pos = node->pos;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    char* name = node->u.gotoExp.id->u.name;
    (this->childs)[0] = IRLeafName(name);
    return this;
}

TreeNode IREmptyStmt(pNode node){
    return IRNil();
}

TreeNode IRExps(pNode node){
    if(node->u.exps.right){
        TreeNode this = newTreeNode();
        this->name = TreeNodeName[ESEQ];
        this->pos = node->pos;
        this->numOfChild = 2;
        this->childs = newNodeList(2);
        (this->childs)[0] = IRHerald(node->u.exps.left);
        (this->childs)[1] = IRHerald(node->u.exps.right);
        return this;
    } else {
        return IRHerald(node->u.exps.left);
    }
}

TreeNode IRAssign(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRHerald(node->u.assignExp.left);
    (this->childs)[1] = IRHerald(node->u.assignExp.right);
    return this;
}

// same to if-else
TreeNode IRTrinary(pNode node){
    currentScope = newScope();
// generate three auto labels
    char* nameT = "TRUE";
    char* nameF = "FALSE";
    char* nameD = "DONE";
    TreeNode labelT = IRAutoLabel(nameT);
    TreeNode labelF = IRAutoLabel(nameF);
    TreeNode labelD = IRAutoLabel(nameD);

    TreeNode this = newTreeNode();
    this->name = TreeNodeName[ESEQ];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
// CJUMP: if(exp==TRUE) then Tlabel else Flabel
    (this->childs)[0] = IRCjump("EQ", node->u.trinaryExp.test, "TRUE", nameT, nameF);
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
    return this;
}

TreeNode IRBinary(pNode node){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[BINOP];
    this->pos = node->pos;
    this->numOfChild = 3;
    this->childs = newNodeList(3);
    (this->childs)[0] = IROp(node->u.binaryExp.op);
    (this->childs)[1] = IRHerald(node->u.binaryExp.left);
    (this->childs)[2] = IRHerald(node->u.binaryExp.right);
    return this;
}

TreeNode IROp(Op operatorr){
    return IRLeafName(OpName[operatorr]);
}

TreeNode IRPreUnary(pNode node){
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
        c->type = INT;
        c->v.intV = 0;
        (this->childs)[2] = IRConst(c);
        return this;
    } else {
        return IRHerald(node->u.preUnaryExp.exp);
    }
}

TreeNode IRConst(Const c){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[CONST];
    this->pos = -1;
    this->numOfChild = 1;
    this->childs = newNodeList(1);
    switch(c->type){
        case INT:
            (this->childs)[0] = IRLeafName(int2string((int)c->v.intV));
            break;
        case FLOAT:
            (this->childs)[0] = IRLeafName(float2string(c->v.floatV));
            break;
        case STRING:
            (this->childs)[0] = IRLeafName(c->v.stringV);
            break;
        case BOOLEAN:
            (this->childs)[0] = IRLeafName(bool2string(c->v.booleanV));
            break; 
    }
    return this;
}

TreeNode IRPostUnary(pNode node){
    return IRPreUnary(node);
}

TreeNode IRCall(pNode node){
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
    return this;
}

TreeNode IRStoreParams(pNode node){
    if(node->kind == A_VOID){
        return IRNil();
    } else {
        // move params to TEMP
        int count = 1;
        if(node->kind == A_ID){
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
            appendName(tempName, count-1);
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
        appendName(tempName, count-1);
        (ptr->childs)[1] = IRLoadT(p, tempName);        
        return this;
    }
}

TreeNode IRLoadT(pNode node, char* tempName){
    TreeNode this = newTreeNode();
    this->name = TreeNodeName[MOVE];
    this->pos = node->pos;
    this->numOfChild = 2;
    this->childs = newNodeList(2);
    (this->childs)[0] = IRTemp(tempName);
    (this->childs)[1] = IRLeafName(node->u.name);
    return this;
}

TreeNode IRFactor(pNode node){
    Const c = newConst();
    switch(node->kind){
        case A_INT:
            c->type = INT;
            c->v.intV = node->u.intVal;
            break;
        case A_FLOAT:
            c->type = FLOAT;
            c->v.floatV = node->u.floatVal;
            break;
        case A_STRING:
            c->type = STRING;
            c->v.stringV = node->u.stringVal;
            break;
        case A_BOOLEAN:
            c->type = BOOLEAN;
            c->v.booleanV = node->u.booleanVal;
            break;
        case A_NUL:
            return IRNil();
        default:
            error();
    }
    return IRConst(c);
}

TreeNode IRHerald(pNode node){
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
            error();
    }
    return IRNil();
}

Const newConst(){
    return (Const)malloc(sizeof(struct const_value_t));
}

void newLabel(char* name){
    Label newL = (Label)malloc(sizeof(*newL));
    newL->name = (char*)malloc(strlen(name)+1);
    strcpy(newL->name, name);
    newL->type = USER;
    newL->next = globalLabelList;
    globalLabelList = newL;
}

void newAutoLabel(char* name){
    appendName(name, currentScope->id);

    Label newL = (Label)malloc(sizeof(*newL));
    newL->name = (char*)malloc(strlen(name)+1);
    strcpy(newL->name, name);
    newL->type = AUTO;
    newL->next = currentScope->autoLabelList;
    currentScope->autoLabelList = newL;
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
    return (TreeNode)malloc(sizeOfTreeNode);
}
// malloc space for a array of tree node pointers
TreeNode* newNodeList(int num){
    return (TreeNode*)malloc(num*sizeof(TreeNode));
}

void appendName(char* head, int num){
    char* itoa = int2string(num);
    int lengthDst = strlen(head);
    int lengthSrc = strlen(itoa);
    strcat(head, itoa);
    head[lengthDst + lengthSrc] = '\0';
}

char* int2string(int num){
    char* ret = (char*)malloc(sizeof(char) * 256);
    sprintf(ret, "%d", num);
    return ret;
}

char* float2string(double f){
    char* ret = (char*)malloc(sizeof(char) * 256);
    sprintf(ret, "%lf", f);
    return ret;
}

char* bool2string(boolean b){
    if(b == TRUE){
        return "TRUE";
    } else {
        return "FALSE";
    }
}
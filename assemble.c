#include "assemble.h"

#define MAX_ASSEMBLE_LENGTH 4096

string instructionName[52] = {
    "MOVE", "LOAD", "STORE",
    "ADD", "SUB", "INC", "DEC", "NOT", "BNOT",
    "MUL", "DIV", "MOD", "BL", "BR", 
    "LE", "GE", "LT", "GT",
    "EQ", "NEQ",
    "BAND", "BXOR", "BOR",
    "AND", "OR", "DOT", "MEM",

    "ADDI", "SUBI", "MULI", "DIVI", "MODI",
    "BLI", "BRI",
    "LEQI", "GEQI", "LTI", "GTI",
    "EQI", "NEQI",
    "BANDI", "BXORI", "BORI",
    "ANDI", "ORI",
    
    "BEQ", "BNE", "BNZ",
    "JUMP", "JA", "JR", "CALL"
};

Assemble instructions[MAX_ASSEMBLE_LENGTH];
string labels[MAX_ASSEMBLE_LENGTH];
int line = 0;
int tempReg = 0;

Assemble* Convert2Assemble(TreeNode root){
    initAss();
    processNode(root);
    copyLabels();
    return instructions;
}

void processNodeName(TreeNode node){
    // name is variable's name!
    // TODO: just register this variable in varlist;
}

int processNodeBinop(TreeNode node){
    Assemble inst = newInst();
    inst->lbl = NULL;

    TreeNode op = node->childs[0];
    TreeNode fact1 = node->childs[1];
    TreeNode fact2 = node->childs[2];

    // post-order iteration
    // if fact1 is const value then it is automic 
    if(strcmp(fact1->name, "CONST")==0){
        TreeNode child = fact1->childs[0];
        if(child->isStr){
            inst->fac2.val.type = STRING_ASS;
            inst->fac2.val.v.stringVal = copyString(child->name);
        } else{
            if(isFloat(child->name)){
                inst->fac2.val.type = FLOAT_ASS;
                inst->fac2.val.v.floatVal = string2float(child->name);
            } else if(isBool(child->name)){
                inst->fac2.val.type = BOOL_ASS;
                inst->fac2.val.v.boolVal = string2bool(child->name);
            }
            else {
                inst->fac2.val.type = INT_ASS;
                inst->fac2.val.v.intVal = string2int(child->name);
            }
        }
        inst->fac2.lbl = inst->fac2.reg = NULL;
    } 
    // if it is a temp(var) then just use var's name as reg name
    else if (strcmp(fact1->name, "TEMP") == 0){
        inst->fac2.reg = copyString(fact1->childs[0]->name);
        inst->fac2.lbl = NULL;
    } 
    // otherwise this operand is a expression, do recursively
    else {
        int regNum = processNode(fact1);
        inst->fac2.reg = appendName("T", regNum);
        inst->fac2.lbl = NULL;
    }
    // if-else cases for fact2, same as fact1
    if(strcmp(fact2->name, "CONST")==0){
        inst->fac3.lbl = inst->fac3.reg = NULL;
        TreeNode child = fact2->childs[0];
        if(child->isStr){
            inst->fac3.val.type = STRING_ASS;
            inst->fac3.val.v.stringVal = copyString(child->name);
        } else{
            if(isFloat(child->name)){
                inst->fac3.val.type = FLOAT_ASS;
                inst->fac3.val.v.floatVal = string2float(child->name);
            } else if(isBool(child->name)){
                inst->fac3.val.type = BOOL_ASS;
                inst->fac3.val.v.boolVal = string2bool(child->name);
            }
            else {
                inst->fac3.val.type = INT_ASS;
                inst->fac3.val.v.intVal = string2int(child->name);
            }
        }
    } 
    // if it is a temp(var) then just use var's name as reg name
    else if (strcmp(fact2->name, "TEMP") == 0){
        inst->fac3.lbl = NULL;
        inst->fac3.reg = copyString(fact2->childs[0]->name);
    } 
    // otherwise this operand is a expression, do recursively
    else {
        int regNum = processNode(fact2);
        inst->fac3.lbl = NULL;
        inst->fac3.reg = appendName("T", regNum);
    }

    // handle THIS node
    if(strcmp(op->name, "PLUS") == 0){   
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = ADDI_ASS;
            inst->type = I;
        } else {
            inst->inst = ADD_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "MINUS") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = SUBI_ASS;
            inst->type = I;
        } else {
            inst->inst = SUB_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "INC") == 0){
        inst->inst = INC_ASS;
            inst->type = R;
    } else if(strcmp(op->name, "DEC") == 0){
        inst->inst = DEC_ASS;
            inst->type = R;
    } else if(strcmp(op->name, "NOT") == 0){
        inst->inst = NOT_ASS;
            inst->type = R;
    } else if(strcmp(op->name, "BNOT") == 0){
        inst->inst = B_NOT_ASS;
            inst->type = R;
    } else if(strcmp(op->name, "TIMES") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = MULI_ASS;
            inst->type = I;
        } else {
            inst->inst = MUL_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "DIV") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = DIVI_ASS;
            inst->type = I;
        } else {
            inst->inst = DIV_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "MOD") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = MODI_ASS;
            inst->type = I;
        } else {
            inst->inst = MOD_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "BLEFT") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = B_LI_ASS;
            inst->type = I;
        } else {
            inst->inst = B_L_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "BRIGHT") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = B_RI_ASS;
            inst->type = I;
        } else {
            inst->inst = B_R_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "LE") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = LEQI_ASS;
            inst->type = I;
        } else {
            inst->inst = LEQ_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "GE") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = GEQI_ASS;
            inst->type = I;
        } else {
            inst->inst = GEQ_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "LT") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = LEI_ASS;
            inst->type = I;
        } else {
            inst->inst = LE_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "GT") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = GTI_ASS;
            inst->type = I;
        } else {
            inst->inst = GT_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "EQ") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = EQI_ASS;
            inst->type = I;
        } else {
            inst->inst = EQ_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "NEQ") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = NEQI_ASS;
            inst->type = I;
        } else {
            inst->inst = NEQ_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "BAND") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = B_ANDI_ASS;
            inst->type = I;
        } else {
            inst->inst = B_AND_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "BXOR") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = B_XORI_ASS;
            inst->type = I;
        } else {
            inst->inst = B_XOR_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "BOR") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = B_ORI_ASS;
            inst->type = I;
        } else {
            inst->inst = B_OR_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "AND") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = ANDI_ASS;
            inst->type = I;
        } else {
            inst->inst = AND_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "OR") == 0){
        if(strcmp(fact1->name, "CONST")==0 ||
            strcmp(fact2->name, "CONST") == 0){
            inst->inst = ORI_ASS;
            inst->type = I;
        } else {
            inst->inst = OR_ASS;
            inst->type = R;
        }
    } else if(strcmp(op->name, "DOT") == 0){
        inst->inst = DOT_ASS;
        inst->type = R;
    } else if(strcmp(op->name, "BRACKET") == 0){
        // if(strcmp(fact1->name, "CONST")==0 ||
        //     strcmp(fact2->name, "CONST") == 0){
        //     inst->inst = BRACKETI_ASS;
        //     inst->type = I;
        // } else {
        //     inst->inst = BRACKET_ASS;
        //     inst->type = R;
        // }
        inst->inst = MEM_ASS;
        inst->type = R;
    }
    inst->fac1.reg = appendName("T", tempReg);
    tempReg ++;
    instructions[line] = inst;
    line ++;
    return tempReg - 1;
}

int processNodeCall(TreeNode node){
    Assemble inst = newInst();
    inst->type = J;
    inst->inst = CALL_ASS;
    inst->lbl = NULL;
    inst->fac1.lbl = copyString(node->childs[0]->name);
    inst->fac1.reg = NULL;
    instructions[line] = inst;
    line ++;

    inst = newInst();
    inst->type = R;
    inst->inst = MOV_ASS;
    inst->fac1.reg = appendName("T", tempReg);
    inst->lbl = NULL;
    tempReg ++;
    inst->fac2.reg = "RO";
    instructions[line] = inst;
    line ++;
    return tempReg - 1;
}

int processNodeEseq(TreeNode node){
    processNode(node->childs[0]);
    Assemble inst = newInst();
    inst->lbl = NULL;
    inst->type = R;
    inst->inst = MOV_ASS;
    inst->fac1.lbl = NULL;
    inst->fac1.reg = appendName("T", tempReg);
    tempReg ++;
    inst->fac2.lbl = NULL;
    if(strcmp(node->childs[1]->name, "TEMP") == 0){
        inst->fac2.reg = copyString(node->childs[1]->childs[0]->name);
        inst->fac3.lbl = NULL;
        inst->fac3.reg = NULL;
    } else{
        int tempReg = processNode(node->childs[1]);
        inst->fac2.reg = appendName("T", tempReg);
        inst->fac2.lbl = NULL;
        inst->fac3.reg = NULL;
    }
    instructions[line] = inst;
    line++;
    return tempReg - 1;
}

void processNodeMove(TreeNode node){
    Assemble inst = newInst();
    inst->type = R;
    inst->inst = MOV_ASS;
    inst->lbl = NULL;
    inst->fac1.reg = copyString(node->childs[0]->childs[0]->name);
    // process right node
    TreeNode rightChild = node->childs[1];
    if(strcmp(rightChild->name, "CONST")==0){
        TreeNode child = rightChild->childs[0];
        if(child->isStr){
            inst->fac2.val.type = STRING_ASS;
            inst->fac2.val.v.stringVal = copyString(child->name);
        } else {
            if(isFloat(child->name)){
                inst->fac2.val.type = FLOAT_ASS;
                inst->fac2.val.v.floatVal = string2float(child->name);
            } else if(isBool(child->name)){
                inst->fac2.val.type = BOOL_ASS;
                inst->fac2.val.v.boolVal = string2bool(child->name);
            }
            else {
                inst->fac2.val.type = INT_ASS;
                inst->fac2.val.v.intVal = string2int(child->name);
            }
        }
        inst->fac2.lbl = NULL;
        inst->fac2.reg = NULL;
    } 
    // if it is a temp(var) then just use var's name as reg name
    else if (strcmp(rightChild->name, "TEMP") == 0){
        inst->fac2.reg = copyString(rightChild->childs[0]->name);
        inst->fac2.lbl = NULL;
    } 
    // otherwise this operand is a expression, do recursively
    else {
        int regNum = processNode(rightChild);
        inst->fac2.reg = appendName("T", regNum);
        inst->fac2.lbl = NULL;
    }
    instructions[line] = inst;
    line++;
}

void processNodeJump(TreeNode node){
    Assemble inst = newInst();
    inst->type = J;
    inst->inst = JMP_ASS;
    inst->lbl = NULL;
    inst->fac1.lbl = copyString(node->childs[0]->name);
    inst->fac1.reg = NULL;

    instructions[line] = inst;
    line++;
}

void processNodeCjump(TreeNode node){
    Assemble inst = newInst();
    int tempReg = processNode(node->childs[1]);
    inst->lbl = NULL;
    inst->type = J;
    inst->inst = BEQ_ASS;
    inst->fac1.lbl = NULL;
    inst->fac1.reg = appendName("T", tempReg);
    inst->fac2.lbl = NULL;
    inst->fac2.reg = NULL;
    inst->fac2.val.type = INT_ASS;
    inst->fac2.val.v.intVal = 0;
    inst->fac3.lbl = copyString(node->childs[3]->name);
    instructions[line] = inst;
    line++;

    inst = newInst();
    inst->lbl = NULL;
    inst->type = J;
    inst->inst = JMP_ASS;
    inst->fac1.lbl = copyString(node->childs[4]->name);
    instructions[line] = inst;
    line++;
}

void processNodeSeq(TreeNode node){
    processNode(node->childs[0]);
    processNode(node->childs[1]);
}

void processNodeLabel(TreeNode node){
    // just copy label name to instruct label field
    // Assemble inst = newInst();
    // inst->lbl = copyString(node->childs[0]->name);
    // instructions[line] = inst;
    labels[line] = copyString(node->childs[0]->name);
}

int processNode(TreeNode node){
    if(strcmp(node->name, "CONST") == 0){
        // shouldn't come to this
        return 0;
    } else if(strcmp(node->name, "NAME") == 0){
        processNodeName(node);
        return 0;
    } else if(strcmp(node->name, "TEMP") == 0){
        // should not come to this branch
        return 0;
    } else if(strcmp(node->name, "BINOP") == 0){
        return processNodeBinop(node);
    } else if(strcmp(node->name, "CALL") == 0){
        return processNodeCall(node);
    } else if(strcmp(node->name, "ESEQ") == 0){
        return processNodeEseq(node);
    } else if(strcmp(node->name, "MOVE") == 0){
        processNodeMove(node);
    } else if(strcmp(node->name, "JUMP") == 0){
        processNodeJump(node);
    } else if(strcmp(node->name, "CJUMP") == 0){

    } else if(strcmp(node->name, "SEQ") == 0){
        processNodeSeq(node);
    } else if(strcmp(node->name, "LABEL") == 0){
        processNodeLabel(node);
    } else if(strcmp(node->name, "NIL") == 0){
        return 0;
    }
    return 0;
}

void copyLabels(){
    for(int i = 0; i< line; i++){
        if(labels[i]){
            instructions[i]->lbl = copyString(labels[i]);
        }
    }
}

// utils
Assemble newInst(){
    Assemble ret= (Assemble)malloc(sizeof(*ret));
    ret->lbl = NULL;
    return ret;
}

string copyString(string str){
    int length = strlen(str);
    string ret = (string)malloc(length + 1);
    strcpy(ret, str);
    ret[length] = '\0';
    return ret;
}

int isFloat(string str){
    int length = strlen(str);
    for(int i = 0; i<length; i++){
        if(str[i] == '.') return 1;
    }
    return 0;
}

int isBool(string str){
    if(strcmp(str, "true") == 0) return 1;
    if(strcmp(str, "false") == 0) return 1;
    return 0;
}

int string2bool(string str){
    if(strcmp(str, "true") == 0) return 1;
    if(strcmp(str, "false") == 0) return 0;
    return 0;
}

double string2float(string str){
    int length = strlen(str);
    int i = 0;
    double ret = 0;
    while(str[i] != '.'){
        ret = ret * 10 + str[i] - '0';
        i++;
    };
    i+=1;
    double exp10 = 0.1;
    while(i< length){
        ret += exp10 * (str[i] - '0');
        exp10 *= 0.1;
        i++;
    }
    return ret;
}

long long string2int(string str){
    int length = strlen(str);
    int i = 0;
    long long ret = 0;
    while(i<length){
        ret = ret*10 + str[i] - '0';
        i++;
    }
    return ret;
}

void initAss(){
    for(int i = 0; i< MAX_ASSEMBLE_LENGTH; i++){
        instructions[i] = NULL;
        labels[i] = NULL;
    }
}

#define BUF_SIZE 0xfff2
char buffer[BUF_SIZE];

char* printAssemble(){
    for(int i = 0; i< BUF_SIZE; i++){
        buffer[i] = 0;
    }
    char* pb = buffer;
    for(int i = 0; i < line; i++){
        if(instructions[i]->lbl == NULL)
            pb+= sprintf(pb, "\t\t\t");
        else 
            pb+= sprintf(pb, "%s:\t", instructions[i]->lbl);
        pb+= sprintf(pb, "%s \t", instructionName[instructions[i]->inst]);
        if(instructions[i]->type == R ){
            pb+=sprintf(pb, "%s", 
                instructions[i]->fac1.reg);
            if(instructions[i]->fac2.reg)
                pb+=sprintf(pb, ", %s", instructions[i]->fac2.reg);
            else{
                switch(instructions[i]->fac2.val.type){
                    case INT_ASS:
                        pb+=sprintf(pb, ", %lld", instructions[i]->fac2.val.v.intVal);
                        break;
                    case FLOAT_ASS:
                        pb+=sprintf(pb, ", %.18lf", instructions[i]->fac2.val.v.floatVal);
                        break;
                    case STRING_ASS:
                        pb+=sprintf(pb, ", %s", instructions[i]->fac2.val.v.stringVal);
                        break;
                    case BOOL_ASS:
                        pb+=sprintf(pb, ", %d", instructions[i]->fac2.val.v.boolVal);
                        break;
                }
            }
            if(instructions[i]->fac3.reg)
                pb+=sprintf(pb, ", %s", instructions[i]->fac3.reg);
        }
        else if(instructions[i]->type == I){
            pb+= sprintf(pb, "%s", instructions[i]->fac1.reg);
            if(instructions[i]->fac2.reg){
                pb+=sprintf(pb,", %s", instructions[i]->fac2.reg);
            }else{
                switch(instructions[i]->fac2.val.type){
                    case INT_ASS:
                        pb+=sprintf(pb, ", %lld", instructions[i]->fac2.val.v.intVal);
                        break;
                    case FLOAT_ASS:
                        pb+=sprintf(pb, ", %.18lf", instructions[i]->fac2.val.v.floatVal);
                        break;
                    case STRING_ASS:
                        pb+=sprintf(pb, ", %s", instructions[i]->fac2.val.v.stringVal);
                        break;
                    case BOOL_ASS:
                        pb+=sprintf(pb, ", %d", instructions[i]->fac2.val.v.boolVal);
                        break;
                }
            }
            if(instructions[i]->fac3.reg){
                pb+=sprintf(pb,", %s", instructions[i]->fac3.reg);
            }else{
                switch(instructions[i]->fac3.val.type){
                    case INT_ASS:
                        pb+=sprintf(pb, ", %lld", instructions[i]->fac3.val.v.intVal);
                        break;
                    case FLOAT_ASS:
                        pb+=sprintf(pb, ", %.18lf", instructions[i]->fac3.val.v.floatVal);
                        break;
                    case STRING_ASS:
                        pb+=sprintf(pb, ", %s", instructions[i]->fac3.val.v.stringVal);
                        break;
                    case BOOL_ASS:
                        pb+=sprintf(pb, ", %d", instructions[i]->fac3.val.v.boolVal);
                        break;
                }
            }
        } else {
            if(instructions[i]->inst == JMP_ASS ||
                instructions[i]->inst == CALL_ASS)
                pb+=sprintf(pb, "%s", instructions[i]->fac1.lbl);
            else{
                pb+=sprintf(pb, "%s", instructions[i]->fac1.reg);
                pb+=sprintf(pb, ", 0");
                pb+=sprintf(pb, ", %s", instructions[i]->fac3.lbl);
            }
        }
        pb+=sprintf(pb, "\n");
    }
    return buffer;
}
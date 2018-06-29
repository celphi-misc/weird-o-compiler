#ifndef __ASSEMBLE_H_
#define __ASSEMBLE_H_

#include "irtree.h"

typedef enum{
    // copy and mem 
    MOV_ASS, LOAD_ASS, STORE_ASS,
    // binary operations (R)
    ADD_ASS, SUB_ASS, INC_ASS, DEC_ASS, NOT_ASS, B_NOT_ASS,
    MUL_ASS, DIV_ASS, MOD_ASS,
    B_L_ASS, B_R_ASS,
    LEQ_ASS, GEQ_ASS, LE_ASS, GT_ASS, 
    EQ_ASS, NEQ_ASS,
    B_AND_ASS, B_XOR_ASS, B_OR_ASS,
    AND_ASS, OR_ASS, DOT_ASS, MEM_ASS,
    // binary operations (I)
    ADDI_ASS, SUBI_ASS, MULI_ASS, DIVI_ASS, MODI_ASS,
    B_LI_ASS, B_RI_ASS,
    LEQI_ASS, GEQI_ASS, LEI_ASS, GTI_ASS, 
    EQI_ASS, NEQI_ASS,
    B_ANDI_ASS, B_XORI_ASS, B_ORI_ASS,
    ANDI_ASS, ORI_ASS,
    // branch and jump (J)
    BEQ_ASS, BNE_ASS, BNZ_ASS, 
    JMP_ASS, JA_ASS, JR_ASS, CALL_ASS
} Instruction;

typedef char* string;
typedef string Reg;
typedef string Lab;

typedef struct assemble_value_t Val;
struct assemble_value_t{
    enum { INT_ASS, FLOAT_ASS, STRING_ASS, BOOL_ASS } type;
    union {
        long long intVal;
        double floatVal;
        string stringVal;
        int boolVal;
    } v;
};

typedef enum{ R, I, J} instType;

typedef struct assemble_instruction_t* Assemble;
struct assemble_instruction_t{
    Lab lbl;
    instType type;
    Instruction inst;
    struct { Reg reg; Lab lbl; } fac1;
    struct { Reg reg; Val val; Lab lbl; } fac2;
    struct { Reg reg; Val val; Lab lbl; } fac3;
};

Assemble* Convert2Assemble(TreeNode root);

int processNode(TreeNode node);
void processNodeName(TreeNode node);
int processNodeBinop(TreeNode node);
int processNodeCall(TreeNode node);
int processNodeEseq(TreeNode node);
void processNodeMove(TreeNode node);
void processNodeJump(TreeNode node);
void processNodeCjump(TreeNode node);
void processNodeSeq(TreeNode node);
void processNodeLabel(TreeNode node);

void copyLabels();

// utils
void initAss();
Assemble newInst();
string copyString(string str);
int isFloat(string str);
int isBool(string str);

int string2bool(string);
double string2float(string);
long long string2int(string);

char* printAssemble();

#endif
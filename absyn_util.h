#ifndef ABSYN_UTIL_H_
#define ABSYN_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include "absyn_types.h"

void* checked_malloc(int size);

void insertVar(A_var varList, A_var p);
void insertFunc(A_func funcList, A_func p);
void insertLabel(A_label labelList, A_label p);

A_var   lookupVar(char* name);
A_func  lookupFunc(char* name);
A_label lookupLabel(char* name);


#define ERROR_VAR_REDEFINE      0x0000
#define ERROR_FUNC_REDEFINE     0x0001
#define ERROR_LABEL_REDEFINE    0x0002
void error(int errorCode);



#endif
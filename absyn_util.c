#include "absyn_util.h"

void* checked_malloc(int size){
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr,"\nRan out of memory!\n");
        exit(1);
    }
    return p;
}

void insertVar(A_var varList, A_var p){
    p->next = varList;
    varList->next = p;
}

void insertFunc(A_func funcList, A_func p){
    p->next = funcList;
    funcList->next = p;
}

void insertLabel(A_label labelList, A_label p){
    p->next = labelList;
    labelList->next = p;
}

A_var lookupVar(char* name){
    A_var p = varList;
    while(p){
        if(!strcmp(name, p->name))
            break;
        p = p->next;
    }
    return p;
}

A_func lookupFunc(char* name){
    A_func p = funcList;
    while(p){
        if(!strcmp(name, p->name)) break;
        p = p->next;
    }
    return p;
}

A_label lookupLabel(char* name){
    A_label p = labelList;
    while(p){
        if(!strcmp(name, p->name)) break;
        p = p->next;
    }
    return p;
}

void error(int errorCode){
    fprintf(stderr, "ERROR: abstract syntax, code: %d", errorCode);
}
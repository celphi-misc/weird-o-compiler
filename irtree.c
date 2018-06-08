#include "irtree.h"

TreeNode IRtree;

TreeNode IRTree(pNode root){
    // initial data structures
    init();
    // start iteration
    if(root->kind != A_BLOCK){
        error(ENTRANCE_TYPE_MISMATCH);
        exit(1);
    }
    IRtree->name = TreeNodeName[ESEQ];
    IRtree->childs = (TreeNode*)malloc(2*sizeof(TreeNode));
    IRtree->childs[0] = 
    // 2 pass: error check
    errorCheck();
    return IRtree;
}

void init(){
    IRtree = (TreeNode)malloc(sizeof(*IRTree));

}
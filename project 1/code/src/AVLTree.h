#ifndef AVL_HEADER
#define AVL_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int val;
    int height;
    //the height of the current node
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

AVLNode* createAVL(AVLNode* root);    //returns the root of the tree
AVLNode* insertAVL(AVLNode* node, int value);  //insert new element into the AVLTree
AVLNode* deleteAVL(AVLNode* node, int value);
AVLNode* rebalanceAVL(AVLNode* node);
AVLNode* leftRotate(AVLNode* node);   //left turn the node
AVLNode* rightRotate(AVLNode* node);  //right turn the node

//above are all the AVL manipulation function
//all of the above actions will return the root of the current local tree

int getHeight(AVLNode* node);
int getBF(AVLNode* node);
int minVal(AVLNode* node);    //minimum value in the right subtree
//above are some of the helper functions

#endif
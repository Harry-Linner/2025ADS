#ifndef BST_HEADER
#define BST_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef struct BSTNode {
    int val;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

BSTNode* createBST(BSTNode* root);
BSTNode* insertBST(BSTNode* node, int value);
BSTNode* deleteBST(BSTNode* node, int value);

int findMin(BSTNode* node);

#endif
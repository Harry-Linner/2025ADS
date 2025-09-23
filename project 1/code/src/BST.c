#include <stdio.h>
#include <stdlib.h>
#include "BST.h"

BSTNode* createBST(BSTNode* root)
{
    int n;
    scanf("%d", &n);
    int* tempArray = (int* )calloc(n, sizeof(int));
    for (int i = 0; i < n; i ++) {
        scanf("%d", tempArray + i);
    }
    for (int i = 0; i < n; i ++) {
        root = insertBST(root, tempArray[i]);
    }
    free(tempArray);
    return root;
}

BSTNode* insertBST(BSTNode* node, int value)
{
    if (node == NULL) {
        BSTNode* currNode = (BSTNode* )calloc(1, sizeof(BSTNode));
        currNode->val = value;
        currNode->left = NULL;
        currNode->right = NULL;
        return currNode;
    } else {
        if (value < node->val) {
            node->left = insertBST(node->left, value);
        } else if (value > node->val){
            node->right = insertBST(node->right, value);
        } else {
            return node;
        }
    }
}

BSTNode* deleteBST(BSTNode* node, int value)
{
    if (node == NULL) {
        return NULL;
    } else if (value < node->val) {
        node->left = deleteBST(node->left, value);
    } else if (value > node->val) {
        node->right = deleteBST(node->right, value);
    } else {
        //this is the node we are looking for
        if (node->left == NULL) {
            BSTNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            BSTNode* temp = node->left;
            free(node);
            return temp;
        } else {
            int rightMin = findMin(node->right);
            node->val = rightMin;
            node->right = deleteBST(node->right, rightMin);
        }
        return node;
    }
}

int findMin(BSTNode* node)
{
    if (node->left != NULL) {
        return findMin(node->left);
    } else {
        return node->val;
    }
}
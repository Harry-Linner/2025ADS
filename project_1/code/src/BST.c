#include <stdio.h>
#include <stdlib.h>
#include "BST.h"

BSTNode* createBST(BSTNode* root)
{
    int n;
    scanf("%d", &n);
    //get the data scale of the input
    int* tempArray = (int* )calloc(n, sizeof(int));
    for (int i = 0; i < n; i ++) {
        scanf("%d", tempArray + i);
        //get the data into the temparray
    }
    for (int i = 0; i < n; i ++) {
        root = insertBST(root, tempArray[i]);
        //insert the variables into the tree
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
        //create the first node if the root is NULL
        return currNode;
    } else {
        if (value < node->val) {
            //we recursively solve the problem
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
        //recursively return the root and operate deletion
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
            //in these two cases, we directly delete the node and return the single-subtree
        } else {
            int rightMin = findMin(node->right);
            node->val = rightMin;
            node->right = deleteBST(node->right, rightMin);
            //here we find the minimum node in the right subtree and replace the node to be deleted with this node
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
    //we use this helper function to find the minimum value in the right subtree
    //however, we need to be sure that the first call must satisfies that the right subtree exists
}
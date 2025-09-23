#include <stdio.h>
#include <stdlib.h>
#include "AVLTree.h"

AVLNode* createAVL(AVLNode* root)
{
    int n;
    //input number
    scanf("%d", &n);
    int* tempArr = (int*)calloc(n, sizeof(int));
    //temporary list
    for (int i = 0; i < n; i ++) {
        scanf("%d", tempArr + i);
    }
    for (int i = 0; i < n; i ++) {
        root = insertAVL(root, tempArr[i]);
    }
    free(tempArr);
    //free the space allocated
    return root;
}

AVLNode* insertAVL(AVLNode* node, int value)
{
    if (node == NULL) {
        AVLNode* currNode = (AVLNode*)calloc(1, sizeof(AVLNode));
        currNode->val = value;
        currNode->height = 0;
        currNode->left = NULL;
        currNode->right = NULL;
        return currNode;
    } else if (value < node->val) {
        //go to the left subtree
        node->left = insertAVL(node->left, value);
        //recursively solve the problem
        //if there are BF issues in the subtree, then it would have been already solved in the called insertAVL()
    } else {
        //go to the right subtree
        node->right = insertAVL(node->right, value);
    }
    //if the problem is solved in the called insertAVL(), then we don't have to solve it here.
    //but if the problem doesn't appear in the subtree, then we would have to solve it here.
    node = rebalanceAVL(node);
    //use rotation to solve the problem
    return node;
}

AVLNode* deleteAVL(AVLNode* node, int value)
{
    //this is a recursively-called function
    //so we need to return the root of the tree where the deletion is already finished
    if (node == NULL) {
        return NULL;
    } else if (value < node->val) {
        node->left = deleteAVL(node->left, value);
    } else if (value > node->val) {
        node->right = deleteAVL(node->right, value);
    } else {
        //this is the case where the node to be deleted is found
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        } else if (node->left == NULL) {
            AVLNode* tmp = node->right;
            free(node);
            return tmp;
        } else if (node->right == NULL) {
            AVLNode* tmp = node->left;
            free(node);
            return tmp;
        } else {
            int minRight = minVal(node->right);
            node->val = minRight;
            node->right = deleteAVL(node->right, minRight);
        }
    }
    //we need to process the inbalance issues
    node = rebalanceAVL(node);
    return node;
}

AVLNode* leftRotate(AVLNode* node)
{
    AVLNode* RL = node->right->left;
    AVLNode* R = node->right;
    R->left = node;
    node->right = RL;
    node->height = getHeight(node);
    R->height = getHeight(R);
    return R;
}
//return the new root

AVLNode* rightRotate(AVLNode* node)
{
    AVLNode* LR = node->left->right;
    AVLNode* L = node->left;
    L->right = node;
    node->left = LR;
    node->height = getHeight(node);
    L->height = getHeight(L);
    return L;
}

AVLNode* rebalanceAVL(AVLNode* node)
{
    node->height = getHeight(node);
    int currBF = getBF(node);
    if (currBF == -2) {
        //left side is shorter
        if (getBF(node->right) < 0) {
            //this is the RR case
            node = leftRotate(node);
        } else {
            //this is the RL case
            node->right = rightRotate(node->right);
            node = leftRotate(node);
        }
        
    } else if (currBF == 2) {
        //right side is shorter
        if (getBF(node->left) > 0) {
            //LL case
            node = rightRotate(node);
        } else {
            //LR case
            node->left = leftRotate(node->left);
            node = rightRotate(node);
        }
    }
    //use rotation to solve the problem
    return node;
}

int getHeight(AVLNode* node)
{
    int leftHeight, rightHeight;
    if (node == NULL) {
        return -1;
        //we define the height of the leaf node as 0
    } else {
        if (node->left == NULL) {
            leftHeight = -1;
        } else {
            leftHeight = node->left->height;
        }

        if (node->right == NULL) {
            rightHeight = -1;
        } else {
            rightHeight = node->right->height;
        }
    }
    if (leftHeight > rightHeight) {
        return 1 + leftHeight;
    } else {
        return 1 + rightHeight;
    }
}

int getBF(AVLNode* node)
{
    if (node == NULL) {
        return 0;
    } // basic case when the node is null
    int leftHeight, rightHeight;
    if (node->left == NULL) {
        leftHeight = -1;
    } else {
        leftHeight = node->left->height;
    }
    if (node->right == NULL) {
        rightHeight = -1;
    } else {
        rightHeight = node->right->height;
    }
    return leftHeight - rightHeight;
}

int minVal(AVLNode* node)
{
    if (node->left == NULL) {
        return node->val;
    } else {
        return minVal(node->left);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include "splay.h"

SplayNode *createnode(int k) {
    SplayNode *new = (SplayNode *)malloc(sizeof(SplayNode));
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;
    new->val = k;
    return new;
}

SplayNode *insert(SplayNode *newnode, SplayNode *root) {
    SplayNode *parent = NULL;
    SplayNode *cur = root;
    while (cur) {
        parent = cur;
        if (newnode->val > cur->val)
            cur = cur->right;
        else if (newnode->val < cur->val)
            cur = cur->left;
        else
            return root; // 不允许重复
    }
    newnode->parent = parent;
    if (!parent)
        return newnode;
    if (newnode->val > parent->val)
        parent->right = newnode;
    else
        parent->left = newnode;
    return root;
}

SplayNode *search(int k, SplayNode *root) {
    SplayNode *cur = root;
    while (cur) {
        if (cur->val == k)
            return cur;
        if (cur->val > k)
            cur = cur->left;
        else
            cur = cur->right;
    }
    return NULL;
}

void rightrotate(SplayNode *root, SplayNode *newnode) {
    SplayNode *nr = newnode->right;
    if (root->parent) {
        SplayNode *grandfather = root->parent;
        if (grandfather->left == root) {
            grandfather->left = newnode;
            newnode->parent = grandfather;
        } else if (grandfather->right == root) {
            grandfather->right = newnode;
            newnode->parent = grandfather;
        }
    } else {
        newnode->parent = NULL;
    }
    newnode->right = root;
    root->parent = newnode;
    root->left = nr;
    if (nr)
        nr->parent = root;
}

void leftrotate(SplayNode *root, SplayNode *newnode) {
    SplayNode *nl = newnode->left;
    if (root->parent) {
        SplayNode *grandfather = root->parent;
        if (grandfather->left == root) {
            grandfather->left = newnode;
            newnode->parent = grandfather;
        } else if (grandfather->right == root) {
            grandfather->right = newnode;
            newnode->parent = grandfather;
        }
    } else {
        newnode->parent = NULL;
    }
    newnode->left = root;
    root->parent = newnode;
    root->right = nl;
    if (nl)
        nl->parent = root;
}

SplayNode* splay(SplayNode *newnode, SplayNode *root) {
    while (newnode->parent != NULL) {
        SplayNode *parent = newnode->parent;
        SplayNode *grandparent = parent->parent;
        if (grandparent == NULL) {
            if (parent->left == newnode)
                rightrotate(parent, newnode);
            else
                leftrotate(parent, newnode);
        } else if (grandparent->left == parent && parent->left == newnode) {
            rightrotate(grandparent, parent);
            rightrotate(parent, newnode);
        } else if (grandparent->right == parent && parent->right == newnode) {
            leftrotate(grandparent, parent);
            leftrotate(parent, newnode);
        } else if (grandparent->left == parent && parent->right == newnode) {
            leftrotate(parent, newnode);
            rightrotate(grandparent, newnode);
        } else if (grandparent->right == parent && parent->left == newnode) {
            rightrotate(parent, newnode);
            leftrotate(grandparent, newnode);
        }
    }
    return newnode;
}

SplayNode *findmax(SplayNode *root) {
    if (!root) return NULL;
    while (root->right)
        root = root->right;
    return root;
}

SplayNode *findmin(SplayNode *root) {
    if (!root) return NULL;
    while (root->left)
        root = root->left;
    return root;
}

SplayNode *delete(SplayNode *root) {
    if (root == NULL)
        return NULL;

    SplayNode *left = root->left;
    SplayNode *right = root->right;
    root->left = root->right = root->parent = NULL;
    free(root);

    if (!left) {
        if (right) right->parent = NULL;
        return right;
    }
    if (!right) {
        left->parent = NULL;
        return left;
    }

    // 两边都不空，左子树最大节点splay到根，再接上右子树
    SplayNode *max = findmax(left);
    left = splay(max, left);
    left->right = right;
    if (right) right->parent = left;
    return left;
}

void free_Splay(SplayNode* node) {
    if (node == NULL) return;
    SplayNode *left = node->left;
    SplayNode *right = node->right;
    node->left = node->right = node->parent = NULL;
    free(node);
    free_Splay(left);
    free_Splay(right);
}

void Traverse(SplayNode *root) {
    if (!root) return;
    Traverse(root->left);
    printf("%d ", root->val);
    Traverse(root->right);
}
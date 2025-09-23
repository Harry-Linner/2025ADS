#ifndef SPLAY_HEADER
#define SPLAY_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node *left;
    struct node *right;
    struct node *parent;
    int val;
} SplayNode;

SplayNode* createnode(int k);
SplayNode* insert(SplayNode *newnode, SplayNode *root);
SplayNode* search(int k, SplayNode *root);
SplayNode* delete(SplayNode *root);
void splay(SplayNode *newnode, SplayNode *root);
void Traverse(SplayNode *root);

#endif
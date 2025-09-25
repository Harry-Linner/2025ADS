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
    if (root == NULL) {
        return newnode;
    }
    if (newnode->val > root->val) {
        root->right = insert(newnode, root->right);
        if (root->right) root->right->parent = root;
    } else if (newnode->val < root->val) {
        root->left = insert(newnode, root->left);
        if (root->left) root->left->parent = root;
    }
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

SplayNode* rightrotate(SplayNode* x) {
    SplayNode* y = x->left;
    if (!y) return x;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
    return y;
}

SplayNode* leftrotate(SplayNode* x) {
    SplayNode* y = x->right;
    if (!y) return x;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return y;
}

SplayNode* splay(SplayNode* x, SplayNode* root) {
    while (x->parent) {
        SplayNode* p = x->parent;
        SplayNode* g = p->parent;
        if (!g) {
            // Zig
            if (p->left == x)
                root = rightrotate(p);
            else
                root = leftrotate(p);
        } else if (g->left == p && p->left == x) {
            // Zig-Zig
            rightrotate(g);
            root = rightrotate(p);
        } else if (g->right == p && p->right == x) {
            // Zig-Zig
            leftrotate(g);
            root = leftrotate(p);
        } else if (g->left == p && p->right == x) {
            // Zig-Zag
            leftrotate(p);
            root = rightrotate(g);
        } else if (g->right == p && p->left == x) {
            // Zig-Zag
            rightrotate(p);
            root = leftrotate(g);
        }
    }
    return x;
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
    
    SplayNode *new_root = NULL;
    
    if (root->left) {
        SplayNode *max = findmax(root->left);
        splay(max, root->left);
        new_root = root->left;
        if (new_root) {
            new_root->right = root->right;
            if (root->right)
                root->right->parent = new_root;
        }
    } else if (root->right) {
        SplayNode *min = findmin(root->right);
        splay(min, root->right);
        new_root = root->right;
        if (new_root) {
            new_root->left = root->left;
            if (root->left)
                root->left->parent = new_root;
        }
    }
    
    free(root);
    return new_root;
}

void Traverse(SplayNode *root) {
    if (!root) return;
    Traverse(root->left);
    printf("%d ", root->val);
    Traverse(root->right);
    return ;
}
// int main()
// {
// 	int i;
//     SplayNode *root=(SplayNode*)malloc(sizeof(SplayNode));
//     SplayNode *newnode=(SplayNode*)malloc(sizeof(SplayNode));
//     SplayNode *target=(SplayNode*)malloc(sizeof(SplayNode));
//     SplayNode *target1=(SplayNode*)malloc(sizeof(SplayNode));
//     root=NULL;
//     newnode=NULL;
//     int n,a[101];
//     char operation[101];
//     scanf("%d",&n);
//     for(i=0;i<n;i++)
//     {
//       scanf("%d %c",&a[i],&operation[i]);      
// 	  newnode=createnode(a[i]);
//       if(operation[i]=='i')
//       {
//         root=insert(newnode,root);
//         splay(newnode,root);
//         root=newnode;
//       //  Traverse(root);
//       }
//       else if(operation[i]=='d')
//       {
//       	target=search(a[i],root);
//         splay(target,root);
//         root=target;
//        root=delete(root);
//       //  Traverse(root);
//       }
//       target1=search(3,root);
//     } 

//     Traverse(root);
// }
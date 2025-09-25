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
SplayNode *insert(SplayNode *newnode,SplayNode *root)
{
    if(root==NULL)
    {
      return newnode;
    }
    newnode->parent=root;
    if(newnode->val>root->val)
      root->right=insert(newnode,root->right);
    else if(newnode->val<root->val)
      root->left=insert(newnode,root->left);
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
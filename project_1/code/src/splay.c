#include <stdio.h>
#include <stdlib.h>
#include "splay.h"

SplayNode *createnode(int k) //initialize a tree whose val is k
{
    SplayNode *new=(SplayNode *)malloc(sizeof(SplayNode));
    new->left=NULL;
    new->right=NULL;
    new->parent=NULL;
    new->val=k;
    return new;
}
SplayNode *insert(SplayNode *newnode,SplayNode *root) //insert a new node following the rule of BST
{
    if(root==NULL)
      return newnode;
    if(newnode->val>root->val) 
    {
      root->right=insert(newnode,root->right);//insert to the right subtree,using recursion
      if(root->right) //avoid that root->right==NULL;
        root->right->parent=root;//establish the parent-child relationship
    } 
    else if(newnode->val<root->val) 
    {
      root->left=insert(newnode,root->left);//insert to the left subtree,using recursion
      if(root->left)//root->right==NULL;
        root->left->parent=root;//establish the parent-child relationship
    }
    return root;
}
SplayNode *search(int k, SplayNode *root) {
    SplayNode *cur=root;
    while(cur) 
    {
        if (cur->val==k)
            return cur;
        if (cur->val>k)
            cur=cur->left;
        else
            cur=cur->right;
    }
    return NULL;
}

void rightrotate(SplayNode *root, SplayNode *newnode) //right rotate the child node 
{
    SplayNode *nr=newnode->right;//record the previous newnode->right
    if (root->parent) //if root has a parent,establish the parent-child relationship between grandparent and newnode
    {
        SplayNode *grandfather=root->parent;
        if (grandfather->left==root) 
        {
            grandfather->left=newnode;
            newnode->parent=grandfather;
        } 
        else if (grandfather->right==root) 
        {
            grandfather->right=newnode;
            newnode->parent=grandfather;
        }
    } 
    else //the parent node is exactly the root
        newnode->parent=NULL;
    newnode->right=root;//change the parent-child relationship between root and newnode
    root->parent=newnode;
    root->left=nr;//establish the parent-child relationship between root and previous newnode->right
    if(nr)
        nr->parent=root;
}

void leftrotate(SplayNode *root, SplayNode *newnode)//left rotate the child node 
{
    SplayNode *nl=newnode->left;//record the previous newnode->left
    if(root->parent) //if root has a parent,establish the parent-child relationship between grandparent and newnode
    {
        SplayNode *grandfather=root->parent;
        if(grandfather->left==root) 
        {
            grandfather->left=newnode;
            newnode->parent=grandfather;
        } 
        else if(grandfather->right==root) 
        {
            grandfather->right=newnode;
            newnode->parent=grandfather;
        }
    } 
    else //the parent node is exactly the root
        newnode->parent=NULL;
    newnode->left=root;//change the parent-child relationship between root and newnode
    root->parent=newnode;
    root->right=nl;//establish the parent-child relationship between root and previous newnode->left
    if(nl)
        nl->parent=root;
}
SplayNode* splay(SplayNode *newnode, SplayNode *root) 
{
    while(newnode->parent!=NULL) 
    {
        SplayNode *parent=newnode->parent;
        SplayNode *grandparent=parent->parent;
        if (grandparent==NULL) //newnode is the son of root;
        {
            if (parent->left==newnode)
                rightrotate(parent,newnode);
            else
                leftrotate(parent,newnode);
        } 
        else if(grandparent->left==parent&&parent->left==newnode) //case "zig-zig"
        {
            rightrotate(grandparent,parent);//first rotate the parent node
            rightrotate(parent,newnode);//then rotate newnode 
        } 
        else if(grandparent->right==parent&&parent->right==newnode) //case "zig-zig"
        {
            leftrotate(grandparent,parent);//first rotate the parent node
            leftrotate(parent,newnode);//then rotate newnode 
        } 
        else if(grandparent->left==parent&&parent->right==newnode) //case "zig-zag"
        {
            leftrotate(parent,newnode);//rotate the newnode node
            rightrotate(grandparent,newnode);//rotate newnode again
        } 
        else if(grandparent->right==parent&&parent->left==newnode) //case "zig-zag"
        {
            rightrotate(parent,newnode);//rotate the newnode
            leftrotate(grandparent,newnode);//rotate newnode again
        }
    }
    return newnode;
}

SplayNode *findmax(SplayNode *root) //according to the rule of BST,the maximum value is at the rightmost position
{
    if (!root) 
        return NULL;
    while(root->right)
        root=root->right;
    return root;
}

SplayNode *findmin(SplayNode *root) //according to the rule of BST,the minimum value is at the leftmost position
{
    if(!root) 
        return NULL;
    while (root->left)
        root=root->left;
    return root;
}
SplayNode *delete(SplayNode *root) //delete the node whose value is k
{
    if(root==NULL)
        return NULL;
    SplayNode *new_root=NULL;
    if (root->left&&root->right) // if the root has both left subtree and right subtree
    {
        SplayNode *max=findmax(root->left);//find the maximum value in the left subtree
        new_root=splay(max, root->left);//splay the max node to the root's left son
        new_root->right=root->right;
        if(root->right)
            root->right->parent=new_root;
        new_root->parent=NULL;
    } 
    else if(root->left) //if the root has only left subtree
    {
        new_root=root->left;
        new_root->parent=NULL;
    } 
    else if(root->right) //if the root has only right subtree
    {
        new_root=root->right;
        new_root->parent=NULL;
    }
    
    free(root);
    return new_root;
}

void Traverse(SplayNode *root)//When programminng splay.c,we use this function to examine the splay tree.
{
    if (!root) return;
    Traverse(root->left);//Traverse the tree using pre-ordered traversal
    printf("%d ", root->val);
    Traverse(root->right);
    return ;
}

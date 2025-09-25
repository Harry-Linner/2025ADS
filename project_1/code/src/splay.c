#include<stdio.h>
#include<stdlib.h>
#include"splay.h"

SplayNode *createnode(int k)
{
    SplayNode *new=(SplayNode *)malloc(sizeof(SplayNode));
    new->left=NULL;
    new->right=NULL;
    new->parent=NULL;
    new->val=k;
    return new;
}
SplayNode *insert(SplayNode *newnode,SplayNode *root)
{
    if(root==NULL)
    {
      return newnode;
    }
    if(newnode->val>root->val)
    {
      root->right=insert(newnode,root->right);
      if(root->right) root->right->parent=root;
    }
    else if(newnode->val<root->val)
    {
      root->left=insert(newnode,root->left);
      if(root->left) root->left->parent=root;
    }
    return root;
}
SplayNode *search(int k,SplayNode *root)
{
	if(!root)
	  return NULL;
	if(root->val==k)
	  return root;
	if(root->val>k)
	  return search(k,root->left);
	return search(k,root->right);
} 
void rightrotate(SplayNode* root,SplayNode *newnode)
{
    SplayNode *nr=newnode->right;
    if(root->parent)
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
    else newnode->parent=NULL;
    newnode->right=root;
    root->parent=newnode;
    root->left=nr;
    if(nr) 
	  nr->parent=root;
    return ;
}
void leftrotate(SplayNode* root,SplayNode *newnode)
{
    SplayNode *nl=newnode->left;
    if(root->parent)
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
    else newnode->parent=NULL;
    newnode->left=root;
    root->parent=newnode;
    root->right=nl;
    if(nl) 
	  nl->parent=root;
	return ;
}
void splay(SplayNode *newnode,SplayNode *root)
{
    while(1)
    {
        SplayNode *parent=newnode->parent;
        if(parent==NULL)
          break;
        else
        {
            if(parent==root)
            {
                if(root->left==newnode)
                {
                 // printf("l ");
                  rightrotate(root,newnode);
                }
                else if(root->right==newnode)
                {
                 // printf("r ");
                  leftrotate(root,newnode); 
                }
            }
            else
            {
                SplayNode *grandparent=parent->parent;
                if(grandparent->left&&grandparent->left->left==newnode) //zig-zig ll
                {
                   //printf("ll ");
                   rightrotate(grandparent,parent);
                   rightrotate(parent,newnode);
                }
                else if(grandparent->left&&grandparent->left->right==newnode) //zig-zag lr
                {
                	//printf("lr ");
                   leftrotate(parent,newnode);
                   rightrotate(grandparent,newnode);
                }
                else if(grandparent->right&&grandparent->right->right==newnode) //zig-zig rr
                {
                	//printf("rr ");
                    leftrotate(grandparent,parent);
                    leftrotate(parent,newnode);
                }
                else if(grandparent->right&&grandparent->right->left==newnode) //zig-zag rl
                {
                //printf("rl ");
                    rightrotate(parent,newnode);
                    leftrotate(grandparent,newnode);
                }
            }
        }
    }
    return ;
}
SplayNode * findmax(SplayNode *root)
{
    if(!root)
      return NULL;
    if(root->right)
      return findmax(root->right);
    return root;
}
SplayNode * findmin(SplayNode *root)
{
    if(!root)
      return NULL;
    if(root->left)
      return findmin(root->left);
    return root;
}
SplayNode *delete(SplayNode*root)
{
    SplayNode *max=(SplayNode*)malloc(sizeof(SplayNode));
  	max=findmax(root->left);
    if(max)
    {
    //	printf("max=%d\n",max->val );
      if(max!=root->left)
	  {	
	    max->parent->right=max->left;
	    if(max->left ) max->left->parent=max->parent;
	    max->left=root->left;
		root->left->parent=max;
      }
	  max->right=root->right;
      max->parent=NULL;
      
      if(root->right) 
	    root->right->parent=max;
	  return max;
    }
    SplayNode *min=(SplayNode*)malloc(sizeof(SplayNode));
    min=findmin(root->right);
    if(min)
    {

      if(min!=root->right)
      {
	    min->parent->left=min->right;
	    if(min->right ) min->right->parent=min->parent;
        min->right=root->right;
        root->right->parent=min; 
		
      }      
     
	  min->parent=NULL;
      min->left=NULL;
     // root=min;
	 return min;
    }
    return NULL;
}
void Traverse(SplayNode *root)
{
    if(!root) return ;
    Traverse(root->left);
    printf("%d ",root->val);
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
#include "func.h"
#include <stdio.h>

Node *recurseInsert(int value, Node *node){
  if (value <= node->value){ /* if it should be on the left */
    if (node->left){  /* recurse on this child */
      return recurseInsert(value, node->left);
    }
    else{/* create it */
      return insertLeft(value, node);
    }
  }
  else{ /* it should be on the right */
    if (node->right){ /* recurse on the child */
      return recurseInsert(value, node->right);
    }
    else{ /* create it */
      return insertRight(value, node);
    }
  }
}

Node *insertNode(int value, Node *root){
  if( !root ){
    return createRoot(value);
  }
  else{
    recurseInsert(value, root);
    return root;
  }
}

Node *search(int value, Node *node){
  if(!node){
    return NULL;
  }
  if(node->value == value){
    return node;
  }
  else if (value < node->value){
    if (node->left){
      return search(value, node->left);
    }
    else{
      return NULL;
    }
  }
  else{
    if (node->right){
      return search(value, node->right);
    }
    else{
      return NULL;
    }
  }
}

Node *removeNode(int value, Node *root){
  Node *node = search(value, root);
  if (!node){ /* nothing to remove */
    printf("not deleting anything");
    return root;
  }
  else{
    Node *new;

    if (!(node->left) && !(node->right)){ /* no children*/
      new = NULL;
    }
    else if (!(node->left)){ /* only a right child */
      new = node->right;
      new->parent = node->parent;
    }
    else if (!(node->right)){ /* only a left child */
      new = node->left;
      new->parent = node->parent;
    }
    else{ /* left and right children */
      /* Grab the next smallest leaf node */
      new = node->left;
      if(new->right){
        while(new->right){
          new=new->right;
        }
        /* Uproot it from its current location */
        if(new->parent){
          new->parent->right = NULL;
        }
      }
      new->left = node->left;
      new->right = node->right;
      new->right->parent = new;
      new->left->parent = new;
      printf("got a problem? %d\n", new->value);
    }
    if(root == node){ /* we deleted the root */
      deleteNode(node);
      return new;
    }
    else{
      Node *parent = node->parent;
      if (parent->left == node){ /* If its the left of its parent*/
        parent->left = new;
      }
      else{ /* if it's the right of its parent */
        parent->right = new;
      }
      deleteNode(node);
      return root;
    }
  }
}

Node *empty(Node *root){
  if(!root){
    return NULL;
  }
  if(root->left){
    empty(root->left);
  }
  if(root->right){
    empty(root->right);
  }
  deleteNode(root);
  return NULL;
}

void iot(Node *root){
  if( !root ){
    return;
  }
  else{
    iot(root->left);
    printf("%d ", root->value);
    iot(root->right);
  }
}
void preot(Node *root){
  if( !root ){
    return;
  }
  else{
    printf("%d ", root->value);
    preot(root->left);
    preot(root->right);
  }
}
void postot(Node *root){
  if( !root ){
    return;
  }
  else{
    postot(root->left);
    postot(root->right);
    printf("%d ", root->value);
  }
}

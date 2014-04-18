#include "func.h"
#include <stdio.h>

int isLeaf(Node *node){
  return node && !(node->left || node->right);
}

Node *recurseInsert(int key, int value, Node *node){
  if (key <= node->key){ /* if it should be on the left */
    if (node->left){  /* recurse on this child */
      return recurseInsert(key, value, node->left);
    }
    else{/* create it */
      return insertLeft(key, value, node);
    }
  }
  else{ /* it should be on the right */
    if (node->right){ /* recurse on the child */
      return recurseInsert(key, value, node->right);
    }
    else{ /* create it */
      return insertRight(key, value, node);
    }
  }
}

Node *insertNode(int key, int value, Node *root){
  if( !root ){
    return createRoot(key, value);
  }
  else{
    recurseInsert(key, value, root);
    return root;
  }
}

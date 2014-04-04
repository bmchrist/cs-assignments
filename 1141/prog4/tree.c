#include <stdlib.h>
#include "tree.h"

Node *createNode(int value, Node *parent){
  Node *node;
  node = malloc(sizeof(Node));
  node->value = value;
  node->parent = parent;
  node->left = NULL;
  node->right = NULL; 
  return node;
}

Node *insertLeft(int value, Node *parent){
  Node *node;
  node = createNode(value, parent);
  parent->left = node;
  return node;
}

Node *insertRight(int value, Node *parent){
  Node *node;
  node = createNode(value, parent);
  parent->right = node;
  return node;
}

Node *createRoot(int value){
  return createNode(value, (void *)'0');
}

void deleteNode(Node *node){
  free(node);
}

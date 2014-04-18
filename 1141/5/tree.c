#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

Node *createNode(int key, int value, Node *parent){
  Node *node;
  node = malloc(sizeof(Node));
  node->key = key;
  node->value = value;
  node->parent = parent;
  node->left = NULL;
  node->right = NULL; 
  return node;
}

Node *insertLeft(int key, int value, Node *parent){
  Node *node;
  node = createNode(key, value, parent);
  parent->left = node;
  return node;
}

Node *insertRight(int key, int value, Node *parent){
  Node *node;
  node = createNode(key, value, parent);
  parent->right = node;
  return node;
}

Node *createRoot(int key, int value){
  return createNode(key, value, (void *)'0');
}

void deleteNode(Node *node){
  free(node);
}

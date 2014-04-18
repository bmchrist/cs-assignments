#ifndef TREE_H
#define TREE_H
typedef struct Node {
  int value;
  int key;
  struct Node *left, *right, *parent;
} Node;

Node *createRoot(int key, int value);
Node *insertLeft(int key, int value, Node *parent);
Node *insertRight(int key, int value, Node *parent);
void deleteNode(Node *node);
#endif

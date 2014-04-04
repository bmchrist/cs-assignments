#ifndef TREE_H
#define TREE_H
typedef struct Node {
  int value;
  struct Node *left, *right, *parent;
} Node;

Node *createRoot(int value);
Node *insertLeft(int value, Node *parent);
Node *insertRight(int value, Node *parent);
void deleteNode(Node *node);
#endif

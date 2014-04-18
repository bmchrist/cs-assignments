#include <stdio.h>

#include "tree.h"
#include "func.h"

/* Read in a node from a file, with the format
 *    key value (separated by some amount of whitespace )
 * and insert into the tree specified by root */
Node *readNode(FILE *file, Node *root){
  int value, key;
  fscanf(file, "%d %d", &value, &key);
  return insertNode(key,value,root);
}
void decode(FILE *file, Node *root){
  char ch;
  Node *cur = root;
  while(1){
    fscanf(file, "%c", &ch);
    if( ch == '2'){
      return;
    }
    else if( ch == '1'){
      cur = cur->right;
    }
    else if(ch == '0'){
      cur = cur->left;
    }
    else if(ch == 10){
      /* Just the line feed character. Cool. */
    }
    else{
      printf("Something went horribly wrong:%c:%x\n",ch,ch);
      return;
    }
    if( isLeaf(cur) ){
      printf("%c", cur->value);
      cur = root;
    }
  }
   /*until 2*/
     /*read bit*/
     /*traverse tree*/
     /*if leaf, print, move pointer back to root*/
}

int main(int argc, char *argv[]){
  FILE * inFile;
  int n; /* Number of items in the tree */
  int temp;
  Node *root = NULL;

  if( argc < 2){
    printf("Not enough arguments\n");
    return 1;
  }
  inFile = fopen(argv[1], "r");
  if( inFile == NULL ){
    printf("File could not be opened\n");
    return 1;
  }

  fscanf(inFile, "%d", &n); /* How many nodes ? */
  if(n == 0){
    printf("Empty huffman tree\n");
    return 1;
  }
  for(temp=0; temp<n; temp++){
    /* Build our tree */
    root = readNode(inFile, root);
  }
  decode(inFile, root);
  fclose(inFile);
}

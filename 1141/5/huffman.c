#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "func.h"

/* Read in a node from a file, with the format
 *    key value (separated by some amount of whitespace )
 * and insert into the tree specified by root */
Node *readNode(FILE *inFile, Node *root){
  int value, key;
  fscanf(inFile, "%d %d", &value, &key);
  return insertNode(key,value,root);
}
/* Take one byte at a time from inFile to decode our message 
 * Output the decoded character to outFile */
void decode(FILE *outFile, FILE *inFile, Node *root){
  char ch;
  Node *cur = root;
  while(1){
    fscanf(inFile, "%c", &ch);
    if( ch == '2'){
      return;
    }
    else if( ch == '1'){
      cur = cur->right;
    }
    else if(ch == '0'){
      cur = cur->left;
    }
    else if(ch == 0){
      printf("Something went horribly wrong\n");
    }
    else{
      /* We'll just ignore anything that's not 0 1 2 or EOF */
      continue;
    }
    if( isLeaf(cur) ){
      fprintf(outFile, "%c", cur->value);
      cur = root;
    }
  }
}

int main(int argc, char *argv[]){
  FILE *inFile, *outFile;
  char *out_path;
  int n; /* Number of items in the tree */
  int temp;
  Node *root = NULL;

  if( argc < 2){
    printf("Not enough arguments\n");
    return 1;
  }
  inFile = fopen(argv[1], "r");
  if( inFile == NULL ){
    printf("Input file could not be opened\n");
    return 1;
  }

  out_path = malloc(strlen(argv[1])+8);
  sprintf(out_path, "%s.decoded", argv[1]);
  outFile = fopen(out_path, "w");
  free(out_path);
  if( outFile == NULL){
    printf("Output file could not be opened");
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
  /* Read in byte by byte and decode and write the decoded data */
  decode(outFile, inFile, root);

  fclose(inFile);
  fclose(outFile);
}

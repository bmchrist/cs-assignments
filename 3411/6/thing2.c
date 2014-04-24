#include <stdlib.h>
#include <stdio.h>
#include "chainvp.h"

int main(int argc, char *argv[]){
  char **list;
  list = (char **)malloc (sizeof(char *) * 4);
  list [0] = NULL;
  list [1] = NULL;
  list [2] = "ls";
  list [3] = "-l";
  list [4] = "./";
  list [5] = NULL; 
  list [6] = "grep"; 
  list [7] = "jpg"; 
  list [8] = NULL; 
  list [9] = "wc"; 
  list [10] = NULL; 
  /*printf("Chain returned: %d\n",chainvp(1, list)); [> just one command <]*/
  chainvp(3, list);
}

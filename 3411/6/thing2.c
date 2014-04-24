#include <stdlib.h> 
#include <stdio.h>
#include "chainvp.h"

int main(int argc, char *argv[]){
  char ** list;
  list = (char **)malloc (sizeof(char *) * 12);
  list [0] = NULL; /* No input file - use stdin */
  list [1] = NULL; /* No output file - use stdout */
  list [2] = "ls";
  list [3] = "-lt";
  list [4] = "/Users/ben/Dropbox/Screenshots";
  list [5] = NULL; /* That is the end of the argument list for the first process */
  list [6] = "grep";
  list [7] = "png";
  list [8] = NULL; /* That is the end of the argument list for the second process */
  list [9] = "head";
  list [10] = "-6";
  list [11] = NULL; /* That is the end of the argument list for the third process */
  printf("Chain returned: %d\n",chainvp(2, list)); /* we will run three commands */
  /* and pipe them together */
}

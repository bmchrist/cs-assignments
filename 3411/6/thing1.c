#include <stdlib.h> 
#include <stdio.h>
#include "chainvp.h"

int main(int argc, char *argv[]){
  char ** list;
  list = (char **)malloc (sizeof(char *) * 12);
  list [0] = "chainvp.c"; /* No input file - use stdin */
  list [1] = "all-is-well"; /* No output file - use stdout */
  list [2] = "grep";
  list [3] = "fork";
  list [4] = NULL;
  list [5] = "wc";
  list [6] = NULL; /* That is the end of the argument list for the first process */
  printf("Chain returned: %d\n",chainvp(2, list)); /* we will run three commands */
  /* and pipe them together */
}

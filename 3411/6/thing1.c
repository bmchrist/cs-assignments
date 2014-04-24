#include <stdlib.h>
#include <stdio.h>
#include "chainvp.h"

int main(int argc, char *argv[]){
  char **list;
  list = (char **)malloc (sizeof(char *) * 4);
  list [0] = "MyInput";
  list [1] = "MyOutput";
  list [2] = "a.out";
  list [3] = NULL; /* That is the end of argument list for the process */
  printf("Chain returned: %d\n",chainvp(1, list)); /* just one command */
}

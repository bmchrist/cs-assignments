#include <stdio.h>

int main(int argc, char *argv[]){
  printf("\nNum %d\n", argc);
  for( int i=0; i<argc; i++){
    printf("%s ",argv[i]);
  }
  printf("\n");
  write(2,"test err",8);
}

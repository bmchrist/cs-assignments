#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int chainvp(int num, char **list){
  printf("Num:%d\n", num);
  int i, idx, start;
  int data;
  /*TODO check if we should redirect stdin stdout*/
  /*printf("%s\n", list[0]); // input file*/
  /*printf("%s\n", list[1]); // output file*/
  int pipes[num+1][2];
  pipe(pipes[0]);
  printf("open pipe[%d]: %d,%d\n", 0,pipes[0][0],pipes[0][1]);
  /* When I write data it should write to pipe */
  /*close(1);*/
  /*data = dup(pipes[0][1]);*/
  /*printf("dup stdin: %d", data);*/
  /* do pipe redirection */
  
  idx=2;
  for(i=0;i<num;i++){
    start = idx;
    while( list[idx] != NULL){
      idx++;
    }
    idx++;
    pipe(pipes[i+1]);
    printf("open pipe[%d]: %d,%d\n",i+1,pipes[i+1][0],pipes[i+1][1]);
    if(fork() == 0){
      /*stdin from read pipe[i] (last process)*/
      /* Get input from last proc's output */
      /* When I read it should come from last proc read */
      close(0);
      data= dup(pipes[i][0]);

      /* We don't need to write to the last proc */
      close(pipes[i][1]);

      /* Write to next proc */
      close(1);
      data= dup(pipes[i+1][1]);
      printf("dup: %d", data);
      /* We don't need to read from the next proc */
      close(pipes[i+1][0]);

      execvp(list[start], &list[start]);
      return -1;
    }
    /* We don't need to read from the last proc */
    close(pipes[i][0]);
    printf("close pipe[%d][%d]\n", i,0);
    /* We don't need to write to the next proc */
    close(pipes[i+1][1]);
    printf("close pipe[%d][%d]\n", i+1,1);
    /* Now the only part of the pipe that is open is read from the next proc
     * which we wil use next */
  }
  close(pipes[0][1]);
  /*close(pipes[i+1][0]);*/

  int status;
  for(i=0;i<num;i++){
    wait(&status);
  }
  char ch;
  int ret = 1;
  while(ret){
    ret = read(pipes[num][0], &ch, 1);
    if(ret > 0){
      write(1, &ch, ret);
    }
  }
  return -1;
}
